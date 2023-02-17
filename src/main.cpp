#include <iomanip>
#include <iostream>

#include "util/json/obj_archive.h"

int main(int argc, char** argv) {
    ObjArchive o;

    int x = 15;
    o.put("x", &x);
    string2 name = "Bob";
    o.put("name", &name);
    printf("%s\n", o.to_str().c_str());

    std::vector<string2> names = {"Bob", "john"};
    o.insert(&names);
    printf("%s\n", o.to_str().c_str());

    o.put("Names", &names);
    printf("%s\n", o.to_str().c_str());

    ObjArchive o2;
    o2.put("o", &o);
    o.put("o2", &o2);
    printf("%s\n", o.to_str().c_str());
    return 0;
}