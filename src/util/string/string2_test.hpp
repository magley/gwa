#include "string2.h"
#include "util/test/test2.h"

#include <stdio.h>
#include <string>

struct string2_Test : Test2 {
protected:
    void run_tests() {
        test01();
        test02();
        test03();
        test04();
        test05();
        test06();
        test07();
        test08();
        test09();
        test10();
        test11();
        test12();
        test13();
    }
private:
    void test01(std::string name = "Should split string") {
        const string2 str = "A A BB";
        const auto parts = str.split(" ");
        ASSERT2(parts == std::vector<string2>{"A", "A", "BB"});
    }

    void test02(std::string name = "Should split string ignoring spaces between specified character-pairs") {
        const string2 str = "Hello {this {will [get]} ignored} and [so will this] and \"this too\" but <not this>.";
        const auto parts = str.split_unless_between(" ", {"[]", "{}", "\"\""});
        ASSERT2(parts == std::vector<string2>{
            "Hello", "{this {will [get]} ignored}", "and", "[so will this]", "and", "\"this too\"",
            "but", "<not", "this>."
        });
    }

    void test03(std::string name = "Should split string and include empty tokens") {
        const string2 str = "A   A   BB  C";
        const auto parts = str.split(" ");
        ASSERT2(parts == std::vector<string2>{"A", "", "", "A", "", "", "BB", "", "C"});
    }

    void test04(std::string name = "Should split string and exclude empty tokens") {
        const string2 str = "A   A   BB  C";
        const auto parts = str.split(" ", false);
        ASSERT2(parts == std::vector<string2>{"A", "A", "BB", "C"});
    }

    void test05(std::string name = "Should replace strings properly") {
        const string2 str = "Replace this {0} this {1} this {2} this {3} and {0} again.";
        const string2 str2 = str
            .replace("{0}", "first")
            .replace("{1}", "second")
            .replace("{2}", "third")
            .replace("{3}", "fourth");
        ASSERT2(str2 == "Replace this first this second this third this fourth and first again.");
    }

    void test06(std::string name = "Should give same string if nothing gets replaced") {
        const string2 str = "Replace nothing.";
        const string2 str2 = str
            .replace("", "AAAAAAA")
            .replace("something", "this other thing");
        ASSERT2(str2 == str);
    }

    void test07(std::string name = "Should trim string properly even if a side has nothing to trim") {
        const string2 str = "AAA   ";
        const string2 str2 = str.trim();
        ASSERT2(str2 == "AAA");
    }

    void test08(std::string name = "Should trim string properly for different whitespace characters") {
        const string2 str = "  \n\n\nAA A      \t ";
        const string2 str2 = str.trim();
        ASSERT2(str2 == "AA A");
    }
    
    void test09(std::string name = "Should join strings like in python") {
        const string2 str = string2::join({"A", "B", "c", "d"}, ", ");
        ASSERT2(str == "A, B, c, d");
    }

    void test10(std::string name = "Split (default: keep empty tokens) and join are inverse functions") {
        const string2 str = "A, B,C, D";
        const auto parts = str.split(", ");
        const string2 str2 = string2::join(parts, ", ");
        ASSERT2(str == str2);
    }

    void test11(std::string name = "Split works if delimiter has multiple characters") {
        const string2 str = ",-1,-2,-3,-,-4,-,-,-5,-";
        const auto parts = str.split(",-", true);
        ASSERT2(parts == std::vector<string2>{
            "", "1", "2", "3", "", "4", "", "", "5", "",
        });
    }

    void test12(std::string name = "Splitting empty string returns an empty list/empty string list if empty token is excluded/included") {
        string2 str = "";
        ASSERT2(str.split(",", false).size() == 0);
        ASSERT2(str.split(",", true) == std::vector<string2>{""});
    }

    void test13(std::string name = "Fast contains-substring works at string bounds") {
        string2 str = "Hello";
        ASSERT2(!str.contains("ello", 0));
        ASSERT2( str.contains("ello", 1));
        ASSERT2(!str.contains("ello", 2));
    }
};