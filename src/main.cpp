#include <iomanip>
#include <iostream>

#include "util/json/obj_archive_test.hpp"
#include "util/string/string2_test.hpp"
#include "util/fixed/fp_test.hpp"

int main(int argc, char** argv) {
    string2_Test string2_test;
    string2_test.run();

    ObjArchive_Test obj_archive_test;
    obj_archive_test.run();

    fp6_Test fp6_test;
    fp6_test.run();
    
    return 0;
}