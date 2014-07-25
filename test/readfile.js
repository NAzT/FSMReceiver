http://stackoverflow.com/questions/14978630/promise-with-q-framework-and-the-callback-pattern-in-node-js

var Q = require("Q");
var fs = require("fs");

var read2 = Q.nfbind(fs.readFile);
var writeFile = Q.denodeify(fs.writeFile);


read2("a.out")
.then(function(content) {
	return writeFile("nat", content);
})
.then(function() {
	console.log(arguments);
})
.done();