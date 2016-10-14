# wavplay
wavplay version for electron

## Change Log
* 1.0.0 - 2016.09.06
  * wavplay-prebuilt version for electron-1.3 and 1.4.
  * win32 version works well. (PlaySound api)
  * mac: use /usr/bin/afplay command.

## Dependencies
* node-pre-gyp

## Installation
```
npm install wavply
```

## How to use
```
const wav = require('wavplay')

// log file is optional.
var ret = wav.init('wav.log')
console.log('init() ' + ret)

ret = wav.open()
console.log('open() ' + ret)

const loop = 1; // 0: noloop, 1: loop
ret = wav.start('sample.wav', loop)
console.log('start() ' + ret)

setTimeout(() => {
    ret = wav.stop()
    console.log('stop() ' + ret)

    ret = wav.close()
    console.log('close() ' + ret)

    ret = wav.exit()
    console.log('exit() ' + ret)
}, 3000)
```
