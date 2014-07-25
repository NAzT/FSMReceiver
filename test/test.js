var serialport = require("serialport")
var SerialPort = serialport.SerialPort;
var fs = require('fs');
var sp = new SerialPort("/dev/tty.usbserial-A9CN715P", {
      baudRate : 9600,
      // dataBits : 8,
      // parity : 'none',
      // stopBits: 1,
      // flowControl : false,
      // parser: serialport.parsers.raw
      parser: serialport.parsers.readline("\r\n") 
 }, true);

var Q = require('q');
var debug = true;

function sleep(ms) {
  // use Q.defer() to create a deferred.
  var deferred = Q.defer();
  setTimeout(function() {
    // setTimeout to resolve the deferred, which will trigger the fulfillment handler of the promise.
    deferred.resolve();
  }, ms);
  // return the promise of the deferred.
  return deferred.promise;
}

var read2 = Q.nfbind(fs.readFile);
var writeFile = Q.denodeify(fs.writeFile);

function writeMessage(filename, content) {
  var deferred = Q.defer();
  var str;
  if (!filename) {
    str = "!";
  }
  else {
    str = filename + ";" + content + ";";
  }

  console.log(str, "to be written.")
  sp.write(str, function(err, results) {
    if (err) {
      deferred.reject(new Error(err));
    }
    else {
      deferred.resolve(results);
    }
  });

  return deferred.promise;
}

sp.close( function(err, resp) {
    console.log("CONTROLLER CLOSE");
});

// var flush = Q.nfbind(sp.flush)
var open = function() {
  var deferred = Q.defer();

  sp.on('open', function() {
    if (debug) {
      console.log("OPEN");
    }

    setTimeout(function() { 
      sp.flush(function(err, resp) {
        console.log("FLUSING..")
        if (err) {
          deferred.reject(new Error(err));
        }
        else {
          deferred.resolve(resp);
          if (debug) {
            console.log("FLUSHED");
          }
        }
      })
    }, 1);


  })

  return deferred.promise;
}


Q.fcall(open)
.then(function() {
  return Q.delay(3000).then(function() {
    return writeMessage(null, "!");
  }) 
})
.then(function() { 
  writeMessage("a.out", "YOXX")
})
.then(function() { 
  writeMessage("a2.out", "HELLO")
})
.then(function() {
  console.log(arguments);
})
.fail(function() {
  console.log(arguments);
})
.done();

sp.on('data', function(data) {
  console.log(data);
});