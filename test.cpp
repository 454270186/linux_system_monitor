#include <iostream>

using namespace std;

int main() {
    const std::string kProcDirectory{"/proc/"};
    const std::string kStatFilename{"/stat"};

    string res = kProcDirectory + kStatFilename;

    cout << res << endl;
}