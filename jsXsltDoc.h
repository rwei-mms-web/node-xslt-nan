#ifndef JS_XSLT_DOC
#define JS_XSLT_DOC


#include <nan.h>

class JsXsltDoc : public Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);

  private:
    explicit JsXsltDoc(xsltStylesheetPtr style);
    ~JsXsltDoc();

    static NAN_METHOD(New);
    static Nan::Persistent<v8::Function> constructor;
};

#endif