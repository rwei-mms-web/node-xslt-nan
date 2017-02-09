#include <nan.h>
#include <libxml/HTMLparser.h>
#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libexslt/exslt.h>
#include <string.h>
#include "scopeguard.h"

#define OBJ_DESTRUCTOR(d) Persistent<Object> _weak_handle = Persistent<Object>::New(self); \
                          _weak_handle.MakeWeak(NULL, d);

using namespace v8;

Handle<Value> useErrors = Nan::New(false);

void jsXmlDocCleanup(Persistent<Value> value, void *) {
    Nan::HandleScope scope;
    //Local<Object> obj = value->ToObject();
    // // EXTERNAL(xmlDocPtr, doc, obj, 0);
    // xmlDocPtr doc = (xmlDocPtr) (Local<External>::Cast(obj->GetInternalField(0))->Value());
    // xmlFreeDoc(doc);
    return;
}

void jsXsltStylesheetCleanup(Persistent<Value> value, void *) {
    Nan::HandleScope scope;
    // Local<Object> obj = value->ToObject();
    // // EXTERNAL(xsltStylesheetPtr, style, obj, 0);
    // xsltStylesheetPtr style = (xsltStylesheetPtr) (Local<External>::Cast(obj->GetInternalField(0))->Value());
    // xsltFreeStylesheet(style);
    return;
}

Handle<Object> jsXmlDoc(xmlDocPtr doc) {
    Nan::EscapableHandleScope scope;
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    Local<ObjectTemplate> tpl = ObjectTemplate::New(isolate);
    tpl->SetInternalFieldCount(1);
    Local<Object> obj = tpl->NewInstance();
    obj->SetInternalField(0, External::New(isolate, doc));
    return scope.Escape(obj);
}

Handle<Object> jsXsltStylesheet(xsltStylesheetPtr style) {
    Nan::EscapableHandleScope scope;
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    Local<ObjectTemplate> tpl = ObjectTemplate::New(isolate);
    tpl->SetInternalFieldCount(1);
    Local<Object> obj = tpl->NewInstance();
    obj->SetInternalField(0, External::New(isolate, style));
    return scope.Escape(obj);
}

void freeArray(char **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

// OBJECT(jsXmlDoc, 1, xmlDocPtr doc)
//     INTERNAL(0, doc)
//     OBJ_DESTRUCTOR(&jsXmlDocCleanup)
//     RETURN_SCOPED(self);
// END

// OBJECT(jsXsltStylesheet, 1, xsltStylesheetPtr style)
//     INTERNAL(0, style)
//     OBJ_DESTRUCTOR(&jsXsltStylesheetCleanup)
//     RETURN_SCOPED(self);
// END

NAN_METHOD(readXmlFile) {
    Nan::HandleScope scope;
    v8::String::Utf8Value str(info[0]);

    xmlDocPtr doc = xmlReadFile(
        *str,
        "UTF-8",
        useErrors->BooleanValue()
            ? XML_PARSE_RECOVER | XML_PARSE_NOERROR
            : 0);
    if (!doc) {
        return Nan::ThrowError("Failed to parse XML");
    }

    info.GetReturnValue().Set(jsXmlDoc(doc));
}

NAN_METHOD(readXmlString) {
    Nan::HandleScope scope;
    v8::String::Utf8Value str(info[0]);

    xmlDocPtr doc = xmlReadMemory(
        *str,
        str.length(),
        NULL,
        "UTF-8",
        useErrors->BooleanValue()
            ? XML_PARSE_RECOVER | XML_PARSE_NOERROR
            : 0);
    if (!doc) {
        return Nan::ThrowError("Failed to parse XML");
    }

    info.GetReturnValue().Set(jsXmlDoc(doc));
}

NAN_METHOD(readXsltFile) {
    Nan::HandleScope scope;
    v8::String::Utf8Value str(info[0]);

    xmlDocPtr doc = xmlReadFile(*str, "UTF-8", 0);

  if (!doc) {
        return Nan::ThrowError("Failed to parse XML");
    }

    ScopeGuard guard = MakeGuard(xmlFreeDoc, doc);

    xsltStylesheetPtr stylesheet = xsltParseStylesheetDoc(doc);
    if (!stylesheet) {
        return Nan::ThrowError("Failed to parse stylesheet");
    }
    guard.Dismiss();

    info.GetReturnValue().Set(jsXsltStylesheet(stylesheet));
}

NAN_METHOD(readXsltString) {
    Nan::HandleScope scope;
    v8::String::Utf8Value str(info[0]);

    xmlDocPtr doc = xmlReadMemory(*str, str.length(), NULL, "UTF-8", 0);

    if (!doc) {
        return Nan::ThrowError("Failed to parse XML");
    }

    ScopeGuard guard = MakeGuard(xmlFreeDoc, doc);

    xsltStylesheetPtr stylesheet = xsltParseStylesheetDoc(doc);
    if (!stylesheet) {
        return Nan::ThrowError("Failed to parse stylesheet");
    }
    guard.Dismiss();

    info.GetReturnValue().Set(jsXsltStylesheet(stylesheet));
}

NAN_METHOD(transform) {

    Local<Object> objStylesheet=info[0]->ToObject();
    Local<Object> objDocument=info[1]->ToObject();

    xsltStylesheetPtr stylesheet = (xsltStylesheetPtr) (v8::Local<v8::External>::Cast(objStylesheet->GetInternalField(0))->Value());
    xmlDocPtr document = (xmlDocPtr) (v8::Local<v8::External>::Cast(objDocument->GetInternalField(0))->Value());

    char** params = (char **)malloc(sizeof(char *) * (1));
    if (!params) {
        return Nan::ThrowError("Failed to allocate memory");
    }
    memset(params, 0, sizeof(char *) * (1));
    ON_BLOCK_EXIT(freeArray, params, 0);

    try {
        xmlDocPtr result = xsltApplyStylesheet(stylesheet, document, (const char **)params);
        if (!result) {
            return Nan::ThrowError("Failed to apply stylesheet");
        }
        ON_BLOCK_EXIT(xmlFreeDoc, result);

        xmlChar *doc_ptr;
        int doc_len;
        xsltSaveResultToString(&doc_ptr, &doc_len, result, stylesheet);

        if (doc_ptr) {
            ON_BLOCK_EXIT(xmlFree, doc_ptr);

            //RETURN_SCOPED(JS_str2((const char *)doc_ptr, doc_len));
            info.GetReturnValue().Set(Nan::New<String>((const char *)doc_ptr, doc_len).ToLocalChecked());
        } else {
            // return v8::String::Empty();
            info.GetReturnValue().Set(Nan::EmptyString());
        }
    } catch (Handle<Value> err) {
        //return err;
        info.GetReturnValue().Set(err);
    }
}

NAN_MODULE_INIT(init) {
    Nan::Set(target, Nan::New("readXmlFile").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(readXmlFile)).ToLocalChecked());

    Nan::Set(target, Nan::New("readXmlString").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(readXmlString)).ToLocalChecked());

    Nan::Set(target, Nan::New("readXsltFile").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(readXsltFile)).ToLocalChecked());

    Nan::Set(target, Nan::New("readXsltString").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(readXsltString)).ToLocalChecked());

    Nan::Set(target, Nan::New("transform").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(transform)).ToLocalChecked());
}

NODE_MODULE(node_xslt_nan, init)
