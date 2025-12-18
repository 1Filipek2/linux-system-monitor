const { spawn } = require('child_process');
const express = require('express');
const http = require('http');
const { Server } = require("socket.io");

const app = express();
const server = http.createServer(app);
const io = new Server(server, {
    cors: { origin: "*" }
});

const monitor = spawn('../monitor');

monitor.stdout.on('data', (data) => {
    const rawData = data.toString().trim();
    
    const lines = rawData.split('\n');
    
    lines.forEach(line => {
        try {
            const jsonData = JSON.parse(line);
            console.log("Dáta z C:", jsonData);
            
            io.emit('stats', jsonData);
        } catch (e) {
        }
    });
});

monitor.stderr.on('data', (data) => {
    console.error(`Chyba z C: ${data}`);
});

server.listen(4000, () => {
    console.log('Server beží na http://localhost:4000');
});