#include "futil.h"
#include <fstream>
#include <sstream>

string2 from_file(const char* file) {
    std::ifstream ff(file);
    std::stringstream ss;
    ss << ff.rdbuf();
    string2 s = ss.str();
    return s;
}

void to_file(const char* file, const string2& str) {
    std::ofstream ff(file);
    ff.write(str.c_str(), str.size());
}
