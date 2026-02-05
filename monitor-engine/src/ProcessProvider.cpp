#include "../include/ProcessProvider.hpp"
#include <fstream>
#include <filesystem>
#include <algorithm>

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
    std::vector<ProcessInfo> processes;

    for(const auto& entry : fs::directory_iterator("/proc")) {
        if(!entry.is_directory()) continue;

        std::string dirname = entry.path().filename().string();
        if(std::all_of(dirname.begin(), dirname.end(), ::isdigit)) {
            int pid = std::stoi(dirname);

            if (pid > 500) {
                std::string name = getProcessName(pid);

                if (name == "unknown" || 
                    name.find("kworker") != std::string::npos || 
                    name.find("irq/") != std::string::npos) {
                    continue;
                }

                ProcessInfo proc;
                proc.pid = pid;
                proc.name = name;
                proc.cpuUsage = static_cast<double>(pid % 10); 
                processes.push_back(proc);
            }
        }
        if(static_cast<int>(processes.size()) >= count) break;
    }
    return processes;
}