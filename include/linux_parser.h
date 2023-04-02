#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

namespace LinuxParser
{
    // Paths
    const std::string kProcDirectory{"/proc/"};
    const std::string kCmdlineFilename{"/cmdline"};
    const std::string kCpuinfoFilename{"/cpuinfo"};
    const std::string kStatusFilename{"/status"};
    const std::string kStatFilename{"/stat"};
    const std::string kUptimeFilename{"/uptime"};
    const std::string kMeminfoFilename{"/meminfo"};
    const std::string kVersionFilename{"/version"};
    const std::string kOSPath{"/etc/os-release"};
    const std::string kPasswordPath{"/etc/passwd"};

    // System
    float MemoryUtilization();
    long UpTime();
    std::vector<int> Pids();
    int TotalProcesses();
    int RunningProcesses();
    std::string OperatingSystem();
    std::string Kernel();

    // CPU
    enum CPUStates
    {
        kUser_ = 0,
        kNice_,
        kSystem_,
        kIdle_,
        kIOwait_,
        kIRQ_,
        kSoftIRQ_,
        kSteal_,
        kGuest_,
        kGuestNice_
    };

    struct Cpu_info {
        std::string user;
        std::string nice;
        std::string system;
        std::string idle;
        std::string iowait;
        std::string irq;
        std::string softirq;
        std::string steal;
        std::string guest;
        std::string guest_nice;
    };

    std::vector<std::string> CpuUtilization();
    long Jiffies();
    long ActiveJiffies();
    long ActiveJiffies(int pid);
    long IdleJiffies();

    // Processes
    std::string Command(int pid);
    std::string Ram(int pid);
    std::string Uid(int pid);
    std::string User(int pid);
    long int UpTime(int pid);
    std::unordered_map<std::string, std::string> PrcsCpuUtilization(int pid);
}; // namespace LinuxParser

#endif