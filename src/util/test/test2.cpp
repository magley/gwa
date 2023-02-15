#include "test2.h"

void Test2::run() {
    run_tests();
    report();
}

bool Test2::assert(std::string name, bool expression, std::string details) {
    results.push_back(Test2Data(name, expression, details));   
    return expression;
}

void Test2::report() const {
    int total = results.size();
    int failed = 0;
    int passed = 0;

    for (const auto& res : results) {
        if (res.passed) {
            passed++;
        } else {
            failed++;
        }
    }

    for (const auto& res : results) {
        if (res.passed) {
            printf(ANSI_GREEN);
            printf("[PASS] %s\n", res.name.c_str());
        } else {
            printf(ANSI_RED);
            printf("[FAIL] %s\n", res.name.c_str());
            printf(ANSI_GRAY);
            printf("\t%s\n", res.failure_details.c_str());           
        }
    }

    printf(ANSI_WHITE);
    printf("=================================================\n");
    if (passed > 0 && failed == 0) {
        printf(ANSI_GREEN);
    } else {
        printf(ANSI_GRAY);
    }
    
    printf("All:  %d\n", total);
    printf("Pass: %d\n", passed);

    if (failed > 0) {
        printf(ANSI_RED);
    }
    printf("Fail: %d\n", failed);

    printf(ANSI_RESET);
}