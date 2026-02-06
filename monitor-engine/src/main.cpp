#include "../include/SystemMonitor.hpp"
#include "../include/ProcessProvider.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <poll.h>

void handleCommands(SystemMonitor& monitor) {
    struct pollfd pfd;
    pfd.fd = 0; 
    pfd.events = POLLIN;

    while (poll(&pfd, 1, 0) > 0) {
        if (pfd.revents & POLLIN) {
            std::string line;
            if (!std::getline(std::cin, line)) break;
            if (line.find("kill ") == 0) {
                try {
                    int pid = std::stoi(line.substr(5));
                    monitor.killProcess(pid);
                } catch (...) {}
            }
        }
    }
}

int main() {
    SystemMonitor monitor;
    ProcessProvider procProvider;

    while (true) {
        handleCommands(monitor);

        auto cpu = monitor.getCPUUsage();
        auto ram = monitor.getMemoryInfo();
        auto procs = procProvider.getTopProcesses(10);

        std::cout << "{"
                  << "\"cpu\": " << cpu << ", "
                  << "\"ram\": " << ram.percent << ", "
                  << "\"ram_used\": " << ram.used_gb << ", "
                  << "\"ram_total\": " << ram.total_gb << ", "
                  << "\"processes\": [";

        for(size_t i = 0; i < procs.size(); ++i) {
            std::cout << "{\"pid\": " << procs[i].pid 
                      << ", \"name\": \"" << procs[i].name 
                      << "\", \"cpu\": " << procs[i].cpuUsage 
                      << ", \"can_kill\": " << (procs[i].canKill ? "true" : "false") << "}";
            if (i < procs.size() - 1) std::cout << ",";
        }

        std::cout << "]}" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}