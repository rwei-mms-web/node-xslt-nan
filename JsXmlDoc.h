#ifndef JSXMLDOC_H
#define JSXMLDOC_H

#include <nan.h>

class JsXmlDoc : public Nan::ObjectWrap
{
  public:
    static NAN_MODULE_INIT(Init);
    explicit JsXmlDoc(xmlDocPtr doc);
    ~JsXmlDoc();

  private:
    static NAN_METHOD(New);
    static Nan::Persistent<v8::Function> constructor;
};

#endif