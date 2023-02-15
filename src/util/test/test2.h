#pragma once

#include <string>
#include <vector>

#ifndef ASSERT2
#define ASSERT2(...)                                            \
    {                                                           \
        if (!assert(name, __VA_ARGS__, #__VA_ARGS__)) return; \
    }
#endif

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
private:
    std::vector<Test2Data> results;
    void report() const;
};