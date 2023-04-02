#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <string>

using std::vector;
using std::string;

class Processor {
public:
    float Utilization(); // TODO: See src/processor.cpp

private:
    // cpu info
    vector<string> cpu_info_{};

    // for CPU utilization
    float idle_{0.0};
    float non_idle_{0.0};
    float total_{0.0};
};

#endif