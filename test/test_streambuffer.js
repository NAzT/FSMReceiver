var streamBuffers = require("stream-buffers");
// Initialize stream
var myReadableStreamBuffer = new streamBuffers.ReadableStreamBuffer({
  frequency: 10,       // in milliseconds.
  chunkSize: 2048     // in bytes.
}); 

// With a buffer
myReadableStreamBuffer.put(aBuffer);

// Or with a string
myReadableStreamBuffer.put("A String", "utf8");