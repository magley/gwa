#include <iostream>
#include "util/string/string2.h"



int main(int argc, char** argv) {
    string2 str01 = "A A  BB";
    for (const string2& s : str01.split(" ")) {
        std::cout << "[" << s << "] ";
    }

    std::cout << "\n";

    string2 str02 = "A {A BB {C D }} BB C";
    for (const string2& s : str02.split_unless_between(" ", {"{}"})) {
        std::cout << "[" << s << "] ";
    }

    std::cout << "\n";

    string2 str03 = "Fizz said \"Hello World\" {to Buzz} though.";
    for (const string2& s : str03.split_unless_between(" ", {"{}", "\"\""})) {
        std::cout << "[" << s << "] ";
    }

    std::cout << "\n";

    string2 str04 = "Hello $1. It's a whole $2 $1.";
    std::cout << str04.replace("$1", "world").replace("$2", "new").replace(" ", "\t");

    std::cout << "\n";

    string2 str05 = "               \t\t\t\n  he        ho  \n\t";
    std::cout << "[" << str05.trim() << "]";

    std::cout << "\n";

    string2 str06 = string2::join({"A", "B", "c", "d"}, ", ");
    std::cout << str06;

    std::cout << "\n";

    string2 str07 = "Testing for iterators";
    for (const char& c : str07) {
        std::cout << c << " ";
    }
    std::cout << "\n";

    for (char& c : str07) {
        c++;
        std::cout << c << " ";
    }
    std::cout << "\n";

    return 0;
}