var fs = require('fs');
var addon = require('bindings')('node_xslt_nan');


try {
var xml = addon.readXmlFile('./xmltest.xml');
var xslt = addon.readXsltFile('./xslttest.xslt');
var result = addon.transform(xslt, xml);
console.log(result); // 'hello world'
} catch (e) {
    console.log
}

try {
var xmlFs = fs.readFileSync('./xmltest.xml', 'utf8');
var xsltFs = fs.readFileSync('./xslttest.xslt', 'utf8');
var xml2 = addon.readXmlString(xmlFs);
var xslt2 = addon.readXsltString(xsltFs);
var result2 = addon.transform(xslt2, xml2);
console.log(result); // 'hello world'
} catch (e) {
    console.log
}