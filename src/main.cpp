#include "ncurses_display.h"
#include "system.h"
#define DEBUG 0

int main() {
    System system;

    #if DEBUG
    while (1) {
        vector<Process> prcs = system.Processes();
        for (int i = 0; i < prcs.size(); i++) {
            prcs[i].CpuUtilization();
        }
    }

    return 0;
    #endif

    NCursesDisplay::Display(system);
}