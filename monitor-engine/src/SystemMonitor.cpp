#include "../include/SystemMonitor.hpp"
#include <fstream>
#include <sstream>
#include <string>

SystemMonitor::SystemMonitor() {
    lastCPUStats = readCPUStats();
}

CPUData SystemMonitor::readCPUStats() {
    CPUData data = {0};
    std::ifstream file("/proc/stat");
    std::string line;

    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cpuLabel;
        ss >> cpuLabel >> data.irq >> data.nice >> data.system >> data.idle
           >> data.iowait >> data.irq >> data.softirq >> data.steal;
    }
    return data;
}

double SystemMonitor::getCPUUsage() {
    CPUData current = readCPUStats();

    auto calculateTotal = [](const CPUData& d) {
        return d.user + d.nice + d.system + d.idle + d.iowait + d.irq + d.softirq + d.steal;
    };

    unsigned long long total1 = calculateTotal(lastCPUStats);
    unsigned long long total2 = calculateTotal(current);
    unsigned long long idle1 = lastCPUStats.idle;
    unsigned long long idle2 = current.idle;

    lastCPUStats = current;

    unsigned long long diffTotal = total2 - total1;
    unsigned long long diffIdle = idle2 - idle1;

    if (diffTotal == 0) return 0.0;
    return (1.0 - (static_cast<double>(diffIdle) / diffTotal)) * 100.0;
}

SystemMonitor::RAMInfo SystemMonitor::getMemoryInfo() {
    RAMInfo info = {0.0, 0.0, 0.0};
    std::ifstream file("/proc/meminfo");
    std::string line;
    long memTotal = 0, memAvailable = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string label;
        long long value;
        
        ss >> label >> value;

        if (label == "MemTotal:") {
            memTotal = value;
        } else if (label == "MemAvailable:") {
            memAvailable = value;
        }
    }

    if(memTotal > 0) {
        long used_kb = memTotal - memAvailable;
        info.total_gb = static_cast<double>(memTotal) / 1024.0 / 1024.0;
        info.used_gb = static_cast<double>(used_kb) / 1024.0 / 1024.0;
        info.percent = (static_cast<double>(used_kb) / memTotal) * 100.0;
    }
    return info;
}