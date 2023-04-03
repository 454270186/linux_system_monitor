#include <unistd.h>
#include <iostream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stof;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    float total_time;
    float elapsed_time;
    float utime;
    float stime;
    float cutime;
    float cstime;
    float starttime;
    float sys_uptime;
    std::unordered_map<string, string> cpu_map = LinuxParser::PrcsCpuUtilization(pid_);
    
    try {
        utime = stof(cpu_map.at("utime"));
        stime = stof(cpu_map.at("stime"));
        cutime = stof(cpu_map.at("cutime"));
        cstime = stof(cpu_map.at("cstime"));
        starttime = stof(cpu_map.at("starttime"));
        sys_uptime = stof(cpu_map.at("system_uptime"));
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 0.0;
    }

    // calculate
    total_time = utime + stime + cutime + cstime;
    elapsed_time = sys_uptime - (starttime / sysconf(_SC_CLK_TCK));

    return ((total_time / sysconf(_SC_CLK_TCK)) / elapsed_time);
}

// TODO: Return the command that generated this process
string Process::Command() { return cmdline_; }

// TODO: Return this process's memory utilization
string Process::Ram() {
    long ram_mb = static_cast<long>(std::stod(ram_)) / 1024;

    return to_string(ram_mb);
}

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return cpu_uti_ > a.cpu_uti_;
}