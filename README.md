# node_xslt_nan

Forked from node_xslt and recompiled using nodejs/nan.

## Installation
```
npm install node_xslt_nan --save
```

**Or:**

In the root directory, run `node-gyp rebuild` to generate `./build/Release/node_xslt_nan.node`

Then:

```JS
var xslt = require('node_xslt_nan')
```

## Requirements

* [libxml2](http://www.xmlsoft.org/) (libxml2-dev package for Debian-based distros)
* [libxslt](http://xmlsoft.org/xslt/index.html) (libxslt-dev package for Debian-based distros)
* [libexslt](http://xmlsoft.org/xslt/EXSLT/) (libxslt-dev package for Debian-based distros)
* xml2-config (Needs to be on PATH)

## API

| Method | Arguments | Return | Description
| :---: | :---: | :---: | ---
| `transform` | `Stylesheet`, `Document`, `string[]` | **string** | Transforms the document


### Example
```JS
xslt.transform(
  xsltString,
  xmlString
)
```
