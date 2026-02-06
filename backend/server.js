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
    output: process.stdout, 
    terminal: false
});

rl.on('line', (line) => {
    try {
        const jsondata = JSON.parse(line);
        io.emit('stats', jsondata);
    } catch (e) {
    }
});

io.on('connection', (socket) => {
    socket.on('kill_process', (data) => {
        if (data && data.pid) {
            process.stdout.write(`kill ${data.pid}\n`);
        }
    });
});

process.stdin.on('end', () => {
    process.exit();
});

httpserver.listen(4000, () => {
    console.error('relay server running on http://localhost:4000');
});