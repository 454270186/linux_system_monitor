#include <string>
#include <iomanip>

#include "format.h"

using std::string;
using std::setfill;
using std::setw;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    long hours = seconds / 3600;
    long minutes = (seconds % 3600) / 60;
    long sec = seconds % 60;

    std::stringstream time;
    time << setfill('0') << setw(2) << hours << ":"
         << setfill('0') << setw(2) << minutes << ":"
         << setfill('0') << setw(2) << sec;

    return time.str();
}