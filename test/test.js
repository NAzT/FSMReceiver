var serialport = require("serialport")
                 var SerialPort = serialport.SerialPort;
var fs = require('fs');
var sp = new SerialPort("/dev/tty.usbserial-A9CN715P",
{
    baudRate : 115200,
    dataBits : 8,
    parity : 'none',
    stopBits: 1,
    flowControl : true,
    // parser: serialport.parsers.raw
parser: serialport.parsers.readline("\r\n")
}, true);

var streamBuffers = require("stream-buffers");
// Initialize stream
var myReadableStreamBuffer = new streamBuffers.ReadableStreamBuffer({
  frequency: 30,       // in milliseconds.
  chunkSize: 20// in bytes.
});


var Q = require('q');
var debug = false;

function sleep(ms)
{
    // use Q.defer() to create a deferred.
    var deferred = Q.defer();
    setTimeout(function()
    {
        // setTimeout to resolve the deferred, which will trigger the fulfillment handler of the promise.
        deferred.resolve();
    }, ms);
    // return the promise of the deferred.
    return deferred.promise;
}

var read2 = Q.nfbind(fs.readFile);
var writeFile = Q.denodeify(fs.writeFile);

function writeMessage(msg)
{
    var deferred = Q.defer();

    console.log("writing ", msg.length, " bytes. ", msg);
    sp.write(msg, function(err, results)
    {
        if (err)
        {
            deferred.reject(new Error(err));
        }
        else
        {
            deferred.resolve(results);
        }
    });

    return deferred.promise;
}

sp.close( function(err, resp)
{
    console.log("CONTROLLER CLOSE");
});

// var flush = Q.nfbind(sp.flush)
var open = function()
{
    var deferred = Q.defer();

    sp.on('open', function()
    {
        if (debug)
        {
            console.log("OPENNED");
        }

        setTimeout(function()
        {
            sp.flush(function(err, resp)
            {
                console.log("FLUSING..")
                if (err)
                {
                    deferred.reject(new Error(err));
                }
                else
                {
                    deferred.resolve(resp);
                    if (debug)
                    {
                        console.log("FLUSHED");
                    }
                }
            })
        }, 1);


    })

    return deferred.promise;
}

myReadableStreamBuffer.on("data", function(data) {
    writeMessage(data);
});

Q.fcall(open)
.then(function()
{
    return Q.delay(3000).then(function()
    {
        console.log("WRITING [0x1b, 0x1b]");
        return writeMessage(new Buffer([0x1b, 0x1b]));
    })
})
// .then(function()
// {
//     var msg = new Buffer("abcdef.out;")
//     console.log("WRITING.. ", msg.toString());
//     // msg.writeUInt8(0x1b, msg.length-1);
//     return writeMessage(msg);
// })
// .then(function()
// {
//     var msg = new Buffer("HELLO;")
//     // msg.writeUInt8(0x1b, msg.length-1);
//     return Q.delay(3000).then(function()
//     {
//         console.log("WRITING.. ", msg.toString());
//         return writeMessage(new Buffer(msg));
//     })
// })
// .then(function() {
//     return Q.delay(1000).then(function()
//     {
//         return writeMessage(new Buffer([0x1b, 0x1b]));
//     })
// })
.then(function()
{
    return Q.delay(2000).then(function()
    {
        var name = new Buffer("hello.txt;");
        var sep = new Buffer([0x1b, 0x1b]);
        var content = new Buffer("hexhehxhexhgogo");
        var msg = Buffer.concat([name, content, sep]);
        return writeMessage(msg);
    })
})
.then(function(){
    return Q.delay(1000).then(function()
    {
        console.log("WRITING.... RESET");
        return writeMessage(new Buffer([0x1b, 0x1b]));
    })
})
.then(function(content) {

    return Q.delay(1000).then(function()
    {
        var name = new Buffer("nat.out;");
        return writeMessage(name);
    });
})
.then(function() {
  return read2("dl.gif");
})
.then(function(content) {
    return Q.delay(5000).then(function()
    {
        // var name = new Buffer("nat.out;");
        var sep = new Buffer([0x1b, 0x1b]);
        var msg = Buffer.concat([content, sep]);
        console.log("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        console.log(msg.toString("HEX"));
        console.log("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        // return writeMessage(msg);
        myReadableStreamBuffer.put(msg);
    });
})
.fail(function()
{
    console.log(arguments);
})
.done();

sp.on('data', function(data)
{
    console.log(">>", data);
});

sp.on('error', function(data) {
    console.log(err, arguments);
})