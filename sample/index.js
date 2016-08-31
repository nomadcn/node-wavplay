const wav = require('../build/Release/win-wavplay')

var ret = wav.init('wav.log');
console.log('init() ' + ret);

ret = wav.open();
console.log('open() ' + ret);

ret = wav.start('sample.wav', 1);
console.log('start() ' + ret);

setTimeout(() => {
    ret = wav.stop()
    console.log('stop() ' + ret)

    ret = wav.close();
    console.log('close() ' + ret);

    ret = wav.exit();
    console.log('exit() ' + ret);
}, 3000);
