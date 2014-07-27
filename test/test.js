var serialport = require("serialport")
                 var SerialPort = serialport.SerialPort;
var fs = require('fs');
var sp = new SerialPort("/dev/tty.usbserial-A9CN715P",
{
    baudRate : 115200,
    // dataBits : 8,
    // parity : 'none',
    // stopBits: 1,
    // flowControl : false,
    // parser: serialport.parsers.raw
parser: serialport.parsers.readline("\r\n")
}, true);

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


Q.fcall(open)
.then(function()
{
    return Q.delay(3000).then(function()
    {
        console.log("WRITING [0x1a, 0x1a]");
        return writeMessage(new Buffer([0x1a, 0x1a]));
    })
})
.then(function()
{
    var msg = new Buffer("abcdef.out;")
    console.log("WRITING.. ", msg.toString());
    // msg.writeUInt8(0x1a, msg.length-1);
    return writeMessage(msg);
})
.then(function()
{
    var msg = new Buffer("HELLO;")
    // msg.writeUInt8(0x1a, msg.length-1);
    return Q.delay(3000).then(function()
    {
        console.log("WRITING.. ", msg.toString());
        return writeMessage(new Buffer(msg));
    })
})
.then(function() {
    return Q.delay(1000).then(function()
    {
        return writeMessage(new Buffer([0x1a, 0x1a]));
    })
})
.then(function()
{
    return Q.delay(2000).then(function()
    {
        var name = new Buffer("hello.txt");
        var sep = new Buffer(";");
        var content = new Buffer("hexhehxhexhgogo");
        var msg = Buffer.concat([name, sep, content, sep]);
        return writeMessage(msg);
    })
})
.then(function(){
    return Q.delay(1000).then(function()
    {
        console.log("WRITING.... RESET");
        return writeMessage(new Buffer([0x1a, 0x1a]));
    })
})
.then(function() {
  return read2("nat");
})
.then(function(content) {

        var name = new Buffer("image.jpg");
        var sep = new Buffer(";");
        console.log(typeof(content))
        var content = new Buffer(content.toString());
        var msg = Buffer.concat([name, sep, content, sep]);
        console.log(msg);
        return writeMessage(msg);
//     console.log("CONTENT...", content.length);
//     var name = new Buffer("nat.out;");
//     var c = new Buffer(content);
//     var end = new Buffer(";");
//     var reset = new Buffer([0x1a, 0x1a]);
//     var msg = new Buffer([name, c, end, reset]);
//   return writeMessage(msg);
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