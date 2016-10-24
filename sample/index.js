const path = '../lib/binding/Release/node-v48-darwin-x64/wavplay';
const wav = require(path);

var audio = '/Users/noconomad/callsync/native_modules/node-wavplay/sample/sample.wav';
var ret = wav.init('wav.log', 3);
console.log('init() ' + ret);

ret = wav.open();
console.log('open() ' + ret);

ret = wav.start(audio, 1);
console.log('start() ' + ret);

process.stdin.on('data', (buf) => {
    var command = buf.toString('utf8').trim();
    console.log('command: ' + command);

    switch (command) {
        case 'quit':
            wav.stop();
            wav.close();
            wav.exit();
            process.exit(0);
            break;

        case 'stop':
            wav.stop();
            wav.close();
            break;

        case 'start':
            wav.open();
            wav.start(audio, 1);
            break;
    }
});
