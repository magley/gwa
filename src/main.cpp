#include <iomanip>
#include <iostream>

#include "util/json/obj_archive.h"
#include "util/json/obj_archive_test.hpp"

int main(int argc, char** argv) {
    ObjArchive_Test obj_archive_test;
    obj_archive_test.run();
    return 0;
}