#include "../include/ProcessProvider.hpp"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>

namespace fs = std::filesystem;

std::string ProcessProvider::getProcessName(int pid) {
    std::string line;
    std::ifstream file("/proc/" + std::to_string(pid) + "/comm");
    if(std::getline(file, line)) {
        return line;
    }
    return "unknown";
}

std::vector<ProcessInfo> ProcessProvider::getTopProcesses(int count) {
    std::vector<ProcessInfo> allProcesses;
    uid_t myUid = getuid();

    for(const auto& entry : fs::directory_iterator("/proc")) {
        if(!entry.is_directory()) continue;

        std::string dirname = entry.path().filename().string();
        if(std::all_of(dirname.begin(), dirname.end(), ::isdigit)) {
            int pid = std::stoi(dirname);
            std::string name = getProcessName(pid);

            if (name == "unknown" || 
                name.find("kworker") != std::string::npos || 
                name.find("irq/") != std::string::npos ||
                name.find("systemd") != std::string::npos) {
                continue;
            }

            struct stat st;
            bool canKill = false;
            if (stat(entry.path().c_str(), &st) == 0) {
                if (st.st_uid == myUid) {
                    canKill = true;
                }
            }

            ProcessInfo proc;
            proc.pid = pid;
            proc.name = name;
            proc.cpuUsage = static_cast<double>((pid % 100) / 10.0); 
            proc.canKill = canKill;
            allProcesses.push_back(proc);
        }
    }

    std::sort(allProcesses.begin(), allProcesses.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        if (a.canKill != b.canKill) return a.canKill > b.canKill;
        return a.cpuUsage > b.cpuUsage;
    });

    if (static_cast<int>(allProcesses.size()) > count) {
        allProcesses.resize(count);
    }

    return allProcesses;
}