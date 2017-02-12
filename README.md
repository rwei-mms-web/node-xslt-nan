# node_xslt_nan

This repo is forked from node_xslt. Code is simplified and rewritten using nodejs/nan.

Tested with node versions: 0.10.36, 0.12.18, 4.3.2, 6.9.4, 7.5.0.

## Installation
```
npm install node_xslt_nan --save
```

**Or:**

In the root directory, run `node-gyp rebuild` to generate `./build/Release/node_xslt_nan.node`

Targeting different node versions
`node-gyp --target=v0.12.18 rebuild`
`node-gyp --target=v4.3.2 rebuild`

Then:

```JS

var xslt = require('node_xslt_nan');
var transformed = xslt.transform('xml content','xslt content');

```

## Requirements

* [libxml2](http://www.xmlsoft.org/) (libxml2-dev package for Debian-based distros)
* [libxslt](http://xmlsoft.org/xslt/index.html) (libxslt-dev package for Debian-based distros)
* [libexslt](http://xmlsoft.org/xslt/EXSLT/) (libxslt-dev package for Debian-based distros)
* xml2-config (Needs to be on PATH)
