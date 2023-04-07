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

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
    float total_time;
    float elapsed_time;
    float utime;
    float stime;
    float cutime;
    float cstime;
    float starttime;
    float sys_uptime;
    std::vector<string> prcs_cpu = LinuxParser::PrcsCpuUtilization(pid_);
    if (prcs_cpu.size() < 4) {
        return 0.0;
    }
    
    utime = stof(prcs_cpu[0]);
    stime = stof(prcs_cpu[1]);
    cutime = stof(prcs_cpu[2]);
    cstime = stof(prcs_cpu[3]);
    starttime = LinuxParser::UpTime(pid_);
    sys_uptime = LinuxParser::UpTime();

    // calculate
    total_time = utime + stime + cutime + cstime;
    elapsed_time = sys_uptime - (starttime / sysconf(_SC_CLK_TCK));

    return ((total_time / sysconf(_SC_CLK_TCK)) / elapsed_time);
}

// Return the command that generated this process
string Process::Command() { return cmdline_; }

// Return this process's memory utilization
string Process::Ram() {
    long ram_mb = static_cast<long>(std::stod(ram_)) / 1024;

    return to_string(ram_mb);
}

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return cpu_uti_ > a.cpu_uti_;
}