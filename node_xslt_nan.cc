#include <nan.h>
#include <libxml/HTMLparser.h>
#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libexslt/exslt.h>
#include <string.h>
#include "scopeguard.h"

using namespace Nan;    

void freeArray(char **array, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(array[i]);
    }
    free(array);
}

NAN_METHOD(transform) {
    v8::String::Utf8Value xmlStr(info[0]);//xml string
    v8::String::Utf8Value xsltStr(info[1]);//xslt string

    xmlDocPtr xmlDoc = xmlReadMemory(*xmlStr, xmlStr.length(), NULL, "UTF-8", 0);

    if (!xmlDoc) {
        return Nan::ThrowError("Failed to parse XML document");
    }

    xmlDocPtr xsltDoc = xmlReadMemory(*xsltStr, xsltStr.length(), NULL, "UTF-8", 0);

    if (!xsltDoc) {
        return Nan::ThrowError("Failed to parse XSLT document");
    }    

    xsltStylesheetPtr xslt = xsltParseStylesheetDoc(xsltDoc);
    
    if (!xslt) {
        return Nan::ThrowError("Failed to parse stylesheet");
    }

    char **params = (char **)malloc(sizeof(char *) * (1));
    if (!params) {
        return Nan::ThrowError("Failed to allocate memory");
    }

    memset(params, 0, sizeof(char *) * (1));

    ON_BLOCK_EXIT(freeArray, params, 0);

    try
    {
        xmlDocPtr result = xsltApplyStylesheet(xslt, xmlDoc, (const char **)params);

        if (!result) {
            return Nan::ThrowError("Failed to apply stylesheet");
        }

        ON_BLOCK_EXIT(xmlFreeDoc, result);

        ON_BLOCK_EXIT(xmlFreeDoc, xmlDoc);

        ON_BLOCK_EXIT(xsltFreeStylesheet, xslt);    

        xmlChar *doc_ptr;

        int doc_len;

        xsltSaveResultToString(&doc_ptr, &doc_len, result, xslt);

        if (doc_ptr) {
            ON_BLOCK_EXIT(xmlFree, doc_ptr);
            
            info.GetReturnValue().Set(Nan::New<v8::String>((const char *)doc_ptr, doc_len).ToLocalChecked());
        }
        else {
            info.GetReturnValue().Set(Nan::EmptyString());
        }
    }
    catch (v8::Handle<v8::Value> err)
    {
        info.GetReturnValue().Set(err);
    }
}

NAN_MODULE_INIT(init)
{
    Nan::Set(target, Nan::New("transform").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(transform)).ToLocalChecked());  
}

NODE_MODULE(node_xslt_nan, init)
