#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>

// c++17 std::filesystem
#include <filesystem>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;
using std::make_pair;

string LinuxParser::OperatingSystem()
{
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value)
            {
                if (key == "PRETTY_NAME")
                {
                    std::replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }
        }
    }
    return value;
}

string LinuxParser::Kernel()
{
    string os, kernel, version;
    string line;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if (stream.is_open())
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    return kernel;
}

// Update this to use std::filesystem
vector<int> LinuxParser::Pids()
{   
    using namespace std::filesystem;
    vector<int> pids;

    path proc_path(kProcDirectory);
    if (exists(proc_path)) {
        for (const auto& entry : directory_iterator(proc_path)) {
            if (is_directory(entry)) {
                path sub_path = entry.path();
                string sub_name = sub_path.filename().string();
                if (std::all_of(sub_name.begin(), sub_name.end(), isdigit)) {
                    int pid = std::stoi(sub_name);
                    pids.push_back(pid);
                }
            }
        }
    }

    return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
    string key, value, unit;
    float mem_total, mem_free;
    string line;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value >> unit;
            if (key == "MemTotal:") {
                mem_total = stof(value);
            }
            if (key == "MemFree:") {
                mem_free = stof(value);
            }
        }
    }

    return (mem_total - mem_free) / mem_total;
}

// Read and return the disk utilization
std::string LinuxParser::DiskUtilization() {
    string file_sys;
    string size;
    string used;
    string avail;
    string use_per;
    string mounted;

    FILE* pipe = popen("df -h", "r");
    if (pipe == NULL) {
        return string{};  
    }

    char buf[128];
    while (fgets(buf, sizeof(buf), pipe) != NULL) {
        std::string line(buf);
        std::istringstream linestream(line);
        linestream >> file_sys >> size >> used >> avail >> use_per >> mounted;
        if (mounted == "/") {
            string ret;
            ret += used + " / ";
            ret += size;
            ret += " ----- ";
            ret += use_per + "\%Used";

            return ret;
        }
    }


    pclose(pipe);
    return string{};
}



// Read and return the system uptime
long LinuxParser::UpTime() {
    long up_time;
    long idle_procs_time;
    string line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);

    if (stream.is_open()) {
        getline(stream, line);
        std::istringstream linestream(line);

        linestream >> up_time >> idle_procs_time; 
    }

    return up_time;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
// Read the first line of /proc/stat and store them
vector<string> LinuxParser::CpuUtilization() {
    vector<string> CPU;
    string cpu;
    string line;
    Cpu_info cpu_info;
    std::ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> cpu;
            if (cpu == "cpu") {
                linestream >> cpu_info.user >> cpu_info.nice
                           >> cpu_info.system >> cpu_info.idle
                           >> cpu_info.iowait >> cpu_info.irq
                           >> cpu_info.softirq >> cpu_info.steal
                           >> cpu_info.guest >> cpu_info.guest_nice;
                CPU.push_back(cpu_info.user);
                CPU.push_back(cpu_info.nice);
                CPU.push_back(cpu_info.system);
                CPU.push_back(cpu_info.idle);
                CPU.push_back(cpu_info.iowait);
                CPU.push_back(cpu_info.irq);
                CPU.push_back(cpu_info.softirq);
                CPU.push_back(cpu_info.steal);
                CPU.push_back(cpu_info.guest);
                CPU.push_back(cpu_info.guest_nice);

                return CPU;
            }
        }
        
    }

    return {};
}

// Read and return the total number of processes
// in /proc/stat
int LinuxParser::TotalProcesses() {
    string key, value;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "processes") {
                    return std::stoi(value);
                }
            }
        }
    }

    return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string key, value;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "procs_running") {
                    return std::stoi(value);
                }
            }
        }
    }

    return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
    string cmdline;
    string pid_file = to_string(pid);
    
    std::ifstream stream(kProcDirectory + pid_file + kCmdlineFilename);
    if (stream.is_open()) {
        getline(stream, cmdline);
        if (cmdline != "") {
            return cmdline;
        }
    }

    return string();
}

// Read and return the memory used by a process in KB
// stored in /proc/[pid]/status
string LinuxParser::Ram(int pid) {
    string key, value;
    string line;
    string pid_file = to_string(pid);
    std::ifstream stream(kProcDirectory + pid_file + kStatusFilename);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "VmSize:") {
                return value;
            }
        }
    }

    return "0.0";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
    string key, value;
    string line;
    string pid_file = to_string(pid);
    std::ifstream stream(kProcDirectory + pid_file + kStatusFilename);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "Uid:") {
                return value;
            }
        }
    }

    return string();
}

// Read and return the user associated with a process
// Get the username assoiciated with the uid
string LinuxParser::User(int pid) {
    string uid = LinuxParser::Uid(pid);
    string username;
    string password;
    string user_uid;
    string line;
    std::ifstream stream(kPasswordPath);

    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);

            linestream >> username >> password >> user_uid;
            if (user_uid == uid) {
                return username;
            }
        }
    }

    return string();
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
    string token; // the value to be skipped
    string line;
    string pid_file = to_string(pid);
    string starttime;

    std::ifstream stream(kProcDirectory + pid_file + kStatFilename);
    if (stream.is_open()) {
        getline(stream, line);
        std::istringstream linestream(line);

        for (int i = 0; i < 21; i++) {
            linestream >> token;
        }

        linestream >> starttime;

        // get current time
        long sys_uptime = LinuxParser::UpTime();


        return sys_uptime - (stol(starttime) / sysconf(_SC_CLK_TCK));
    }
    
    return 0;
}

// Read and return the cpu utilization of specific process
// in /proc/[pid]/stat:
// need #14, #15, #16, #17, #22
// in /proc/uptime:
// need #1_uptime_of_system
std::vector<std::string> LinuxParser::PrcsCpuUtilization(int pid) {
    string token; // value to be skipped
    string line;
    string pid_file = to_string(pid);
    string utime; // #14
    string stime; // #15
    string cutime; // #16
    string cstime; // #17
    // string starttime; // #22
    // string sys_uptime; // system uptime
    vector<string> prcs_cpu;

    std::ifstream stream(kProcDirectory + pid_file + kStatFilename);
    if (stream.is_open()) {
        getline(stream, line);
        std::istringstream linestream(line);

        for (int i = 0; i < 13; i++) {
            linestream >> token;
        }
        
        linestream >> utime >> stime >> cutime >> cstime;

        // starttime = to_string(LinuxParser::UpTime(pid));
        // sys_uptime = to_string(LinuxParser::UpTime());

        prcs_cpu = {utime, stime, cutime, cstime};

        // return map;
        return prcs_cpu;
    }

    return {};
}
