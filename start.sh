#!/bin/bash

trap "rm -f cmd_pipe; kill 0" EXIT

echo "[INFO] Compiling C++ monitor-engine..."
(cd monitor-engine && make)

if [ $? -ne 0 ]; then
    echo "[ERROR] C++ Compilation failed."
    exit 1
fi

mkfifo cmd_pipe

echo "[INFO] Starting Vite development server..."
(cd frontend && npm run dev) &

sleep 2

echo "[INFO] Starting Interactive Data Pipeline..."

tail -f cmd_pipe | ./monitor-engine/monitor_bin | node backend/server.js > cmd_pipe