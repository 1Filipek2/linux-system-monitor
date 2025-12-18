const express = require('express');
const http = require('http');
const { server } = require("socket.io");
const readline = require('readline');

const app = express();
const httpserver = http.createserver(app);
const io = new server(httpserver, {
    cors: { origin: "*" }
});

const rl = readline.createinterface({
    input: process.stdin,
    terminal: false
});

rl.on('line', (line) => {
    try {
        const jsondata = json.parse(line);
        
        console.log("data received from monitor:", jsondata);
        
        io.emit('stats', jsondata);
    } catch (e) {
        // silent catch for malformed json
    }
});

process.stdin.on('end', () => {
    console.log('input stream pipe closed');
});

httpserver.listen(4000, () => {
    console.log('relay server running on http://localhost:4000');
});