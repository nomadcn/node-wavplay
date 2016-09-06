const wav = require('../build/Release/wavplay')

var ret = wav.init('wav.log', 3);
console.log('init() ' + ret);

ret = wav.open();
console.log('open() ' + ret);

ret = wav.start('/Users/noconomad/callsync/native_modules/wavplay/sample/sample.wav', 1);
console.log('start() ' + ret);

process.stdin.on('data', (buf) => {
    var text = buf.toString('utf8');
    if (text.trim() === 'quit')
    {
        wav.stop();
        wav.close();
        wav.exit();
        process.exit(0);
    }
});
