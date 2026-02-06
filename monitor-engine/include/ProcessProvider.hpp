#ifndef PROCESS_PROVIDER_HPP
#define PROCESS_PROVIDER_HPP

#include <vector>
#include <string>

struct ProcessInfo {
    int pid;
    std::string name;
    double cpuUsage;
    bool canKill;
};

class ProcessProvider {
    private:
        std::string getProcessName(int pid);
    public:
        std::vector<ProcessInfo> getTopProcesses(int count);
};

#endif