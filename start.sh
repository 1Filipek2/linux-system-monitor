#!/bin/bash

trap "kill 0" EXIT

echo "[INFO] Compiling C monitor..."
gcc monitor.c -o monitor

if [ $? -ne 0 ]; then
    echo "[ERROR] C Compilation failed."
    exit 1
fi

echo "[INFO] Starting Vite development server..."
(cd frontend && npm run dev) &

echo "[INFO] Starting Data Pipeline..."
./monitor | node backend/server.js