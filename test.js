var fs = require('fs');
var addon = require('bindings')('node_xslt_nan');
var SegfaultHandler = require('segfault-handler');

SegfaultHandler.registerHandler("crash.log");

try {
var xmlFs = fs.readFileSync('./xmltest.xml', 'utf8');
var xsltFs = fs.readFileSync('./xslttest.xslt', 'utf8');

var result2 = addon.transform(xmlFs, xsltFs);
console.log(result2);
} catch (e) {
    console.log
}