#include "string2.h"
#include "util/test/test2.h"

#include <stdio.h>
#include <string>

struct string2_Test : Test2 {
protected:
    void run_tests() {
        test01("Should pass");
    }
private:
    void test01(std::string name) {
        ASSERT2(true);
    }
};