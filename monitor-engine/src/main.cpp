#include "../include/SystemMonitor.hpp"
#include "../include/ProcessProvider.hpp"

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    SystemMonitor monitor;
    ProcessProvider procProvider;

    while (true) {
        auto cpu = monitor.getCPUUsage();
        auto ram = monitor.getMemoryInfo();
        auto procs = procProvider.getTopProcesses(5);

        std::cout << "{"
                  << "\"cpu\": " << cpu << ", "
                  << "\"ram\": " << ram.percent << ", "
                  << "\"ram_used\": " << ram.used_gb << ", "
                  << "\"ram_total\": " << ram.total_gb << ", "
                  << "\"processes\": [";

        for(size_t i = 0; i < procs.size(); ++i) {
            std::cout << "{\"pid\": " << procs[i].pid 
                      << ", \"name\": \"" << procs[i].name 
                      << "\", \"cpu\": " << procs[i].cpuUsage << "}";
            if (i < procs.size() - 1) std::cout << ",";
        }

        std::cout << "]}" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}