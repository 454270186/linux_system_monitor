#include <iostream>
#include <filesystem>
using namespace std;
using namespace std::filesystem;

int main() {
    const std::string kProcDirectory{"/proc/"};
    const std::string kStatFilename{"/stat"};

    path proc_path("/proc/");

    for (const auto& entry : directory_iterator(proc_path)) {
        if (is_directory(entry)) {
            path sub_path = entry.path();
            string sub_name = sub_path.filename().string();
            cout << "sub_name: " << sub_name << endl;
        }
    }
}