// #include "JsXmlDoc.h"

// Nan::Persistent<v8::Function> JsXmlDoc::constructor;

// NAN_MODULE_INIT(JsXmlDoc::Init) {
//   v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
//   tpl->SetClassName(Nan::New("JsXmlDoc").ToLocalChecked());
//   tpl->InstanceTemplate()->SetInternalFieldCount(1);
//   constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
//   Nan::Set(target, Nan::New("JsXmlDoc").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
// }

// JsXmlDoc::JsXmlDoc(xmlDocPtr doc) {

// }

// JsXmlDoc::~JsXmlDoc() {

// }

// NAN_METHOD(JsXmlDoc::New) {
//   if (info.IsConstructCall()) {    
//     Local<Object> doc=info[0]->ToObject();
//     JsXmlDoc *obj = new JsXmlDoc(doc);
//     obj->Wrap(info.This());
//     info.GetReturnValue().Set(info.This());
//   } else {
//     // Invoked as plain function `MyObject(...)`, turn into construct call.
//     const int argc = 1; 
//     v8::Local<v8::Object> argv[argc] = {info[0]};
//     v8::Local<v8::Function> cons = Nan::New(constructor);
//     info.GetReturnValue().Set(cons->NewInstance(argc, argv));
//   }
// }
