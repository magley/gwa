#pragma once

#include <sstream>
#include <string>
#include <vector>

#ifndef _TEST2_HELPFUL_ASSERTS
#define _TEST2_HELPFUL_ASSERTS

// If it says `name` not found, the test function needs to define (a string) called `name`. 
#define ASSERT2(...)                                            \
    {                                                           \
        if (!assert(name, __VA_ARGS__, #__VA_ARGS__)) return; \
    }

#define ASSERT2_EQ(expected, result)                                        \
{                                                                           \
    if (!assert_eq(name, expected, result)) return;                      \
}

#endif

#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_WHITE "\033[0;37m"
#define ANSI_GRAY "\033[0;39m"
#define ANSI_RESET "\033[0m"

struct Test2Data {
    std::string name;
    bool passed;
    std::string failure_details;

    Test2Data() {}
    Test2Data(std::string name, bool passed, std::string failure_details): 
        name(name), passed(passed), failure_details(failure_details) {}
};

struct Test2 {
    void run();
protected:
    virtual void run_tests() = 0;
    bool assert(std::string name, bool expression, std::string details);

    template<typename T>
    bool assert_eq(std::string name, T expected, T result) {
        bool eq = expected == result;
        std::string details = "";
        if (!eq) {
            std::stringstream ss;
            ss << "Expected:\n\t" << expected << "\ngot:\n\t" << result;
            details = ss.str();
        }
        return assert(name, eq, details);
    }

    template<typename T>
    bool assert_eq(std::string name, std::vector<T> expected, std::vector<T> result) {
        bool eq = expected == result;
        std::string details = "";
        if (!eq) {
            std::stringstream ss;
            ss << "Expected:\n\t[";
            for (int i = 0; i < expected.size() - 1; i++) {
                ss << expected[i] << ", ";
            }
            ss << expected[expected.size() - 1] << "]";

            #define WRITE_RESULT_ELEMENT(i)\
                if (i < expected.size() && result[i] != expected[i]) {\
                    ss << ANSI_YELLOW;\
                } else {\
                    ss << ANSI_RESET;\
                }\
                ss << result[i];

            ss << "\n\tResult:\n\t[";
            for (int i = 0; i < result.size() - 1; i++) {
                WRITE_RESULT_ELEMENT(i);
                ss << ", ";
            }

            WRITE_RESULT_ELEMENT(result.size() - 1);

            ss << ANSI_RESET;
            ss << "]";
            details = ss.str();

            #undef WRITE_RESULT_ELEMENT
        }
        return assert(name, eq, details);
    }
private:
    std::vector<Test2Data> results;
    void report() const;
};