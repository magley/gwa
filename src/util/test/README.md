# test2 - micro testing framework for C++
## Usage
---
```c++
// myTest.hpp
#include "test2.h"
class MyTestSuite : public test2::Test2 {
public:
    void run_tests() { // override from Test2::run_tests()
        EXECUTE_TEST(always_passes);
        EXECUTE_NAMED(is_zero, "0 is 0", 0);
        EXECUTE_NAMED(is_zero, "this will fail", 1248);
        EXECUTE_NAMED(arrays_equal, "arrays should be equal");
    }
private:
    void is_zero(int i) {
        ASSERT2_EQ(0, i);
    }
    
    void always_passes() {
        ASSERT2(true);
    }
    
    void arrays_equal() {
        std::vector<int> v1 = {0, 1, 2, 3};
        std::vector<int> v2 = {0, 1, 2, 3};
        ASSERT2_EQ(v1, v2);
    }
};
```

```c++
// main.cpp
#include "myTest.hpp"
int main() {
    MyTestSuite myTestSuite;
    myTestSuite.run();
    return 0;
}
```
Steps:
1) Create a `class` that inherits from `test2::Test2`.
2) Override the `run_tests()` method.
3) Create tests (basic functions, with `ASSERT2_*` macros).
4) Run test from `run_tests()` with `EXECUTE_TEST` or `EXECUTE_NAMED` macros.
5) Create instance of the newly-made class.
6) Call `.run()` on the test suite instance.

## Design limitations
---
Since C++ lacks mechanisms for reflection, we can choose between:
1) Manually calling tests within suites, and suites within the program:
    `+ cleaner syntax`
    `+ faster compile times`
    `+ more control`
    `+ decipherable compile-time error messages`
    `+ dynamic execution`
    `- same test executing mutliple times when copy-pasting code`
    `- code redundancy`
    `- the program itself is the test runner (no "middleware")`
2) Using macros to automatize the test writing/execution process:
    `+ minimal redundancy`
    `+ test suites written like in other languages`
    `+ tester can focus on writing tests`
    `- macro magic`
    `- slower compile times`
    `- source code is hard to read/maintain`
    
`test2` opted for the 1st solution contrary to other popular and more powerful frameworks for the sake of faster compile times and easier-to-understand code.

## Design guide
---

Test suite headers should only be included in `main.cpp` and only during testing.
Because of the above, it is reccommended to write the entire test suite (including definition) in `.hpp` files.
Test suites should not inherit from other test suites (except for `test2::Test2`). 
Exposing the `test2` namespace is discouraged.

## Inner workings
---
`Test2` is an abstract class representing a test suite. When inheriting this class, you must override `void run_tests()`.
Other functions you may override are: `void before_all()`, `void after_all()`, `void before_each()`, `void after_each()`.
`before_all` and `after_all` are automatically by `Test2::run()` between the call of `run_tests()`. `before_each` and `after_each` are called by the macro `EXECUTE_TEST`/`EXECUTE_NAMED`.

`current_test_name` is an `std::string` holding the name of the current test. `EXECUTE_*` macros set this variable before running each test. The name is derived from the function (in the case of `EXECUTE_TEST`), or from the second argument of the macro (in the case of `EXECUTE_NAMED`).

Tests can hold any number of parameters, in which case they are to be passed as arguments to `EXECUTE_*` macros.

`results` is a collection of `Test2Data` instances holding info about every test ran from the suite.
`ASSERT2_*` macros call the `assert_*` methods which insert the test result into the vector. Once the testing finishes, the results are printed to standard output.