#ifndef SYSTEM_MONITOR_HPP
#define SYSTEM_MONITOR_HPP

#include <string>
#include <vector>

struct CPUData {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
};

class SystemMonitor {
    private:
        CPUData lastCPUStats;
        CPUData readCPUStats();
    public:
        SystemMonitor();

        double getCPUUsage();

        struct RAMInfo {
            double percent;
            double used_gb;
            double total_gb;
        };

        RAMInfo getMemoryInfo();
};
#endif