#!/bin/bash

trap "kill 0" EXIT

echo "[INFO] Compiling C++ monitor-engine..."
(cd monitor-engine && make)

if [ $? -ne 0 ]; then
    echo "[ERROR] C++ Compilation failed."
    exit 1
fi

echo "[INFO] Starting Vite development server..."
(cd frontend && npm run dev) &

sleep 2

echo "[INFO] Starting Data Pipeline (C++ -> Node.js)..."
./monitor-engine/monitor_bin | node backend/server.js