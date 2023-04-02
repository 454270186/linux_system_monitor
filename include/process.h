#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
    Process(int pid) : pid_(pid) {
        cmdline_ = LinuxParser::Command(pid_);
        ram_ = LinuxParser::Ram(pid_);
        user_ = LinuxParser::User(pid_);
        uptime_ = LinuxParser::UpTime(pid_);
        cpu_uti_ = CpuUtilization();
    }

    int Pid();                             
    std::string User();                    
    std::string Command();                 
    float CpuUtilization();                
    std::string Ram();                     
    long int UpTime();                     
    bool operator<(Process const &a) const;

    // TODO: Declare any necessary private members
private:
    int pid_;
    std::string cmdline_;
    std::string ram_;
    std::string user_;
    long int uptime_;
    float cpu_uti_;
};

#endif