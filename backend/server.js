const express = require('express');
const http = require('http');
const { Server: socketserver } = require("socket.io");
const readline = require('readline');

const app = express();
const httpserver = http.createServer(app);
const io = new socketserver(httpserver, {
    cors: { origin: "*" }
});

const rl = readline.createInterface({
    input: process.stdin,
    terminal: false
});

rl.on('line', (line) => {
    try {
        const jsondata = JSON.parse(line);
        io.emit('stats', jsondata);
    } catch (e) {
        // silent catch
    }
});

process.stdin.on('end', () => {
    console.log('input stream pipe closed');
});

httpserver.listen(4000, () => {
    console.log('relay server running on http://localhost:4000');
});