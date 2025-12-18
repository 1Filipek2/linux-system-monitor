# linux system monitor aquarium

![dashboard](docs/dashboard.png)

this project is a real-time system resource monitor for linux that visualizes system processes as an aquarium. it consists of a high-performance c backend, a node.js relay server, and a react frontend.

## architecture

the system operates using a three-tier pipeline:
1. **backend (c)**: reads system metrics directly from /proc/stat and /proc/meminfo. it outputs real-time data as a json stream to stdout.
2. **relay (node.js)**: receives the json stream via unix pipe and broadcasts data to clients using socket.io.
3. **frontend (react)**: a web dashboard that renders processes as a dynamic aquarium, featuring real-time charts and prioritized process sorting.

## prerequisites

- gcc compiler
- node.js (v18 or higher)
- npm or yarn
- linux environment (for /proc filesystem access)

## project structure

- `monitor.c`: c source code for system telemetry.
- `backend/server.js`: node.js websocket relay server.
- `frontend/`: react application source code.
- `start.sh`: automation script for system execution.

## installation

1. **frontend dependencies**
   ```bash
   cd frontend
   npm install