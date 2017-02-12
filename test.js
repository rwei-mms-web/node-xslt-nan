"use strict";
//require('heapdump');
var fs = require('fs');
var addon = require('bindings')('node_xslt_nan');
var memwatch = require('memwatch-next');

var xmlFs = fs.readFileSync('./xmltest.xml', 'utf8');
var xsltFs = fs.readFileSync('./xslttest.xslt', 'utf8');

// memwatch.on('leak', function (info) {
//     console.error(info);
//     if (!hd) {
//         hd = new memwatch.HeapDiff();
//     } else {
//         var diff = hd.end();
//         console.error(util.inspect(diff, true, null));
//         hd = null;
//     }
// });

// memwatch.on('stats', function (stats) {
//     console.log(stats);
// });

function GcThenGetHeapSize() {    
    try {
        global.gc();
    } catch (e) {
        console.log("You must run program with 'node --expose-gc index.js' or 'npm start'");
        process.exit();
    }

    var heapUsed = process.memoryUsage().heapUsed;
    console.log("Program is using " + heapUsed + " bytes of Heap.")
}

setInterval(() => {
    var result = addon.transform(xmlFs, xsltFs);
    console.log(result);
}, 2);
setInterval(GcThenGetHeapSize, 2000);