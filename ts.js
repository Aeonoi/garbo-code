/// <reference types="node" />
var inputData = '';
process.stdin.on('data', function (chunk) {
    inputData += chunk.toString();
});
process.stdin.on('end', function () {
    console.log(inputData.trim());
});
