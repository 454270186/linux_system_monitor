#include <iostream>
#include <string>
#include "processor.h"
#include "linux_parser.h"

using namespace LinuxParser;
using std::stof;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    cpu_info_ = LinuxParser::CpuUtilization();
    if (cpu_info_.size() <= 0) {
        std::cerr << "Fail to get cpu info" << std::endl;
    }

    idle_ = stof(cpu_info_[CPUStates::kIdle_]) + stof(cpu_info_[CPUStates::kIOwait_]);
    non_idle_ = stof(cpu_info_[CPUStates::kUser_]) + stof(cpu_info_[CPUStates::kNice_])+
                stof(cpu_info_[CPUStates::kSystem_]) + stof(cpu_info_[CPUStates::kIRQ_])+
                stof(cpu_info_[CPUStates::kSoftIRQ_]) + stof(cpu_info_[CPUStates::kSteal_]);

    total_ = idle_ + non_idle_;

    return (total_ - idle_) / total_;
}