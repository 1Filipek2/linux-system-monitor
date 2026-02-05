#ifndef PROCESS_PROVIDER_CPP
#define PROCESS_PROVIDER_CPP

#include <vector>
#include <string>

struct ProcessInfo {
    int pid;
    std::string name;
    double cpuUsage;
};

class ProcessProvider {
    private:
        std::string getProcessName(int pid);
    public:
        std::vector<ProcessInfo> getTopProcesses(int count);
};

#endif