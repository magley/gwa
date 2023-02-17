#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace test2 {

#ifndef _TEST2_HELPFUL_ASSERTS
#define _TEST2_HELPFUL_ASSERTS

#define ASSERT2(...)                                \
{                                                   \
    if (!assert(__VA_ARGS__, #__VA_ARGS__)) return; \
}

#define ASSERT2_EQ(expected, result)                \
{                                                   \
    if (!assert_eq(expected, result)) return;       \
}

#define EXECUTE_TEST(test, ...)         \
{                                       \
    current_test_name = #test;          \
    before_each();                      \
    test(__VA_ARGS__);                  \
    after_each();                       \
}

#define EXECUTE_NAMED(test, name, ...)  \
{                                       \
    current_test_name = name;           \
    before_each();                      \
    test(__VA_ARGS__);                  \
    after_each();                       \
}
#endif

#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_WHITE "\033[0;37m"
#define ANSI_GRAY "\033[0;39m"
#define ANSI_RESET "\033[0m"

class Test2Data {
public:
    std::string name;
    bool passed;
    std::string failure_details;

    Test2Data() {}
    Test2Data(std::string name, bool passed, std::string failure_details): 
        name(name), passed(passed), failure_details(failure_details) {}
};

class Test2 {
public:
    void run();
protected:
    virtual void run_tests() = 0;
    virtual void before_all();
    virtual void after_all();
    virtual void before_each();
    virtual void after_each();
    bool assert(bool expression, std::string details);

    template<typename T>
    bool assert_eq(T expected, T result) {
        bool eq = expected == result;
        std::string details = "";
        if (!eq) {
            std::stringstream ss;
            ss << "Expected:\n\t" << expected << "\n\tResult:\n\t" << result;
            details = ss.str();
        }
        return assert(eq, details);
    }

    template<typename T>
    bool assert_eq(std::vector<T> expected, std::vector<T> result) {
        #define WRITE_EXPECTED_ELEMENT(i)\
            if (i >= result.size() || result[i] != expected[i]) {\
                ss << ANSI_RED;\
            } else {\
                ss << ANSI_RESET;\
            }\
            ss << expected[i];
        #define WRITE_RESULT_ELEMENT(i)\
            if (i >= expected.size() || result[i] != expected[i]) {\
                ss << ANSI_RED;\
            } else {\
                ss << ANSI_RESET;\
            }\
            ss << result[i];

        bool eq = expected == result;
        std::string details = "";
        if (!eq) {
            std::stringstream ss;
            ss << "Expected:\n\t[";
            for (int i = 0; i < expected.size() - 1; i++) {
                WRITE_EXPECTED_ELEMENT(i);
                ss << ", ";
            }
            WRITE_EXPECTED_ELEMENT(expected.size() - 1);
            ss << ANSI_RESET << "]";

            ss << "\n\tResult:\n\t[";
            for (int i = 0; i < result.size() - 1; i++) {
                WRITE_RESULT_ELEMENT(i);
                ss << ", ";
            }
            WRITE_RESULT_ELEMENT(result.size() - 1);
            ss << ANSI_RESET << "]";

            ss << "\n\n";
            for (int i = 0; i < expected.size(); i++) {
                if (i < result.size() && result[i] != expected[i]) {
                    ss << ANSI_YELLOW << "\texpected[" << i << "]: " << ANSI_RESET << expected[i]
                        << ANSI_YELLOW << "\tresult[" << i << "]: " << ANSI_RESET << result[i] << "\n";
                }
            }
            if (expected.size() < result.size()) {
                ss << "\texpected.size() < result.size()\n";
            } else if (expected.size() > result.size()) {
                ss << "\texpected.size() > result.size()\n";
            }

            details = ss.str();
        }
        #undef WRITE_RESULT_ELEMENT
        #undef WRITE_EXPECTED_ELEMENT
        return assert(eq, details);
    }
    std::string current_test_name = "!no test!";
private:
    std::vector<Test2Data> results;
    void report() const;
};

} // namespace test2