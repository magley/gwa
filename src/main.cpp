#include <iomanip>
#include <iostream>

#include "util/string/string2.h"
#include "util/string/string2_test.h"

int main(int argc, char** argv) {
    string2_Test string_tests;
    string_tests.run();
    return 0;
    // string2 str01 = "A A  BB";
    // for (const string2& s : str01.split(" ")) {
    //     std::cout << "[" << s << "] ";
    // }

    // std::cout << "\n";

    // string2 str02 = "A {A BB {C D }} BB C";
    // for (const string2& s : str02.split_unless_between(" ", {"{}"})) {
    //     std::cout << "[" << s << "] ";
    // }

    // std::cout << "\n";

    // string2 str03 = "Hello {this {will [get]} ignored} and [so will this] and \"this too\" but <not this>.";
    // for (const string2& s : str03.split_unless_between(" ", {"{}", "[]", "\"\""})) {
    //     std::cout << "[" << s << "] ";
    // }

    // std::cout << "\n";

    // string2 str04 = "Hello $1. It's a whole $2 $1.";
    // std::cout << str04.replace("$1", "world").replace("$2", "new").replace(" ", "\t");

    // std::cout << "\n";

    // string2 str05 = "               \t\t\t\n  he        ho  \n\t";
    // std::cout << "[" << str05.trim() << "]";

    // std::cout << "\n";

    // string2 str06 = string2::join({"A", "B", "c", "d"}, ", ");
    // std::cout << str06;

    // std::cout << "\n";

    // string2 str07 = "Testing for iterators";
    // for (const char& c : str07) {
    //     std::cout << c << " ";
    // }
    // std::cout << "\n";

    // for (char& c : str07) {
    //     c++;
    //     std::cout << c << " ";
    // }
    // std::cout << "\n";

    string2 str08 = ",-1,-2,-3,-,-4,-,-,-5,-";
    for (string2 s : str08.split(",-", false)) {
        std::cout << "[" << s << "] ";
    }
    std::cout << "\n";

    string2 str09 = "";
    std::cout << str09.split(",").size() << " ";
    std::cout << str09.split(",", false).size() << " ";
    std::cout << "\n";

    string2 str10 = "Hello";
    std::cout << str10.contains("ello", 0) << str10.contains("ello", 1) << str10.contains("ello", 2);
    std::cout << "\n";

    string2 str11 = "Hello heLlo\the^ll)o \nho ho_ HO\t\tho";
    std::cout << str11 << "\n"
              << str11.upper() << "\n"
              << str11.lower() << "\n"
              << str11.title();
    std::cout << "\n";

    string2 str12 = "1.3";
    std::cout << std::setprecision(18) << str12.to_i() << "\n"
              << str12.to_l() << "\n"
              << str12.to_f() << "\n"
              << str12.to_d();
    std::cout << "\n";

    return 0;
}