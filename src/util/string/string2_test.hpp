#include "string2.h"
#include "util/test/test2.h"

#include <stdio.h>
#include <string>

class string2_Test : public test2::Test2 {
protected:
    void run_tests() {
        EXECUTE_NAMED(test01, "Should split string");
        EXECUTE_NAMED(test02, "Should split string ignoring spaces");
        EXECUTE_NAMED(test03, "Should split string and include empty tokens");
        EXECUTE_NAMED(test04, "Should split string and exclude empty tokens");
        EXECUTE_NAMED(test05, "Should replace strings properly");
        EXECUTE_NAMED(test06, "Replace nothing -> same string result");
        EXECUTE_NAMED(test07, "Should trim, one side has nothing to trim");
        EXECUTE_NAMED(test08, "Should trim, different whitespace characters");
        EXECUTE_NAMED(test09, "Should join strings like in python");
        EXECUTE_NAMED(test10, "Split (default: keep empty tokens) =~ join");
        EXECUTE_NAMED(test11, "Split with multiple-character delimiters");
        EXECUTE_NAMED(test12, "Splitting empty string exluding empty tokens");
        EXECUTE_NAMED(test13, "Splitting empty string including empty tokens");
        EXECUTE_NAMED(test14, "Fast contains-substring works at string bounds");
    }
private:
    void test01() {
        const string2 str = "A A BB";
        const auto parts = str.split(" ");
        ASSERT2_EQ((std::vector<string2>{"A", "A", "BB"}), parts);
    }

    void test02() {
        const string2 str = "1 {22 {3 [4]} 5} 6 [7] 8 \"9\" <10 11>.";
        const auto parts = str.split_unless_between(" ", {"[]", "{}", "\"\""});
        ASSERT2_EQ((std::vector<string2>{
            "1", "{22 {3 [4]} 5}", "6", "[7]", "8", 
            "\"9\"", "<10", "11>."
        }), parts);
    }

    void test03() {
        const string2 str = "A  A   BB";
        const auto parts = str.split(" ");
        ASSERT2_EQ((std::vector<string2>{"A", "", "A", "", "", "BB"}), parts);
    }

    void test04() {
        const string2 str = "A   A   BB  C";
        const auto parts = str.split(" ", false);
        ASSERT2_EQ((std::vector<string2>{"A", "A", "BB", "C"}), parts);
    }

    void test05() {
        const string2 str = "Replace this {0} this {1} and {0} again.";
        const string2 str2 = str
            .replace("{0}", "first")
            .replace("{1}", "second");
        const string2 ss = "Replace this first this second and first again.";
        ASSERT2_EQ(ss, str2);
    }

    void test06() {
        const string2 str = "Replace nothing.";
        const string2 str2 = str
            .replace("", "AAAAAAA")
            .replace("something", "this other thing");
        ASSERT2_EQ(str, str2);
    }

    void test07() {
        const string2 str = "AAA   ";
        const string2 str2 = str.trim();
        ASSERT2_EQ(string2("AAA"), str2);
    }

    void test08() {
        const string2 str = "  \n\n\nAA A      \t ";
        const string2 str2 = str.trim();
        ASSERT2_EQ(string2("AA A"), str2);
    }
    
    void test09() {
        const string2 str = string2::join({"A", "B", "c", "d"}, ", ");
        ASSERT2_EQ(string2("A, B, c, d"), str);
    }

    void test10() {
        const string2 str = "A, B,C, D";
        const auto parts = str.split(", ");
        const string2 str2 = string2::join(parts, ", ");
        ASSERT2_EQ(str2, str);
    }

    void test11() {
        const string2 str = ",-1,-2,-3,-,-4,-,-,-5,-";
        const auto parts = str.split(",-", true);
        ASSERT2_EQ((std::vector<string2>{
            "", "1", "2", "3", "", "4", "", "", "5", "",
        }), parts);
    }

    void test12() {
        string2 str = "";
        ASSERT2_EQ(0, (int)str.split(",", false).size());
    }
    
    void test13() {
        string2 str = "";
        ASSERT2_EQ(str.split(",", true), std::vector<string2>{""});
    }

    void test14() {
        string2 str = "Hello";
        ASSERT2(str.contains("ello", 1));
    }
};