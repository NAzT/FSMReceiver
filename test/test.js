var serialport = require("serialport")
var SerialPort = serialport.SerialPort;
var sp = new SerialPort("/dev/tty.usbserial-A9CN715P", {
      baudRate : 9600,
      // dataBits : 8,
      // parity : 'none',
      // stopBits: 1,
      // flowControl : false,
      parser: serialport.parsers.raw
      // parser: serialport.parsers.readline("\r\n") 
 }, true);

sp.close( function(err, resp) {
          console.log("CONTROLLER CLOSE");
});

sp.on('open', function() {
  console.log("OPEN")
  sp.flush(function(err, resp) { 
    console.log(err, resp);
    sp.write("nat.txt", function(err, results) {
      console.log('err ' + err);
      console.log('results ' + results);
    });
  });
});

sp.on('data', function(data) {
  console.log('data received: ' + data);
}); 

  // sp.open(function () {
  //   console.log('open');
  // });

// });
// 