# test2 - micro testing framework for C++
## Usage
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
Since C++ lacks mechanisms for reflection, we can choose between:
1) Manually calling tests within suites, and suites within the program:
```
	+ cleaner syntax
	+ faster compile times
	+ more control
	+ decipherable compile-time error messages
	+ dynamic execution
	- same test executing mutliple times when copy-pasting code
	- code redundancy
	- the program itself is the test runner (no "middleware")
```
2) Using macros to automatize the test writing/execution process:
```
	+ minimal redundancy
	+ test suites written like in other languages
	+ tester can focus on writing tests
	- macro magic
	- slower compile times
	- source code is hard to read/maintain
```
`test2` opted for the 1st solution contrary to other popular and more powerful frameworks for the sake of faster compile times and easier-to-understand code.

## Design guide

Test suite headers should only be included in `main.cpp` and only during testing.

Because of the above, it is recommended to write the entire test suite (including definition) in `.hpp` files.

Test suites should not inherit from other test suites (except for `test2::Test2`).

Exposing the `test2` namespace is discouraged.

## Inner workings

A test suite is a class inheriting `test2::Test2`. Method `run()` (the only method the test runner should be exposed to) internally calls `run_tests()` which the tester must override. Additionally, `before_all` and `after_all` are called implicitly. 

To call `before_each` and `after_each`, instead of calling the test functions in `run_tests()` directly, use the `EXECUTE_*` macro family, which takes care of calling `before_each` and `after_each`. Aside from that, the macros modify the internal variable `current_test_name` which is used for documenting test results. By default, each test's name is the name of the function being called, unless a custom name is specified (using `EXECUTE_NAMED()`), in which case the tester must be wary of name clashing.

Tests can be parameterized by passing specific values in the `EXECUTE_*` macros.

Assertions work by checking a single boolean value based on the condition specified by the tester, and documenting the result in an internal collection of test results. If there are multiple assertions in a single test, any negative test will override the previous results of that test and terminate the test's execution.

Exception assertions wrap the given code in a `try-catch` block, catching a specified exception. If the exception is not thrown, the test fails, otherwise the test may succeed. The tester has the option of performing additional code once the given exception is caught, including specialized assertions which can cause the test to fail.

```c++
ASSERT2_THROWS(exception_name, code, after_catch);

// Roughly translates to: 

try {
	code;
} catch(exception_name ex) {
	after_catch;
		
	PASS();
};
FAIL();
```

Once all tests are finished with execution, `run()` prints the test results to standard output.

## Features

#### Test lifecycle methods

```c++
class Test : public test2::Test2 {
	void before_all() { // Called once, when Test::run() is called by the client.
	}
	void after_all() { // Called once, after all tests execute, before reports.
	}
	void before_each() { // Called before each test, use EXECUTE_* macros in run_tests().
	}
	void after_each() { // Called after each test, use EXECUTE_* macros in run_tests().
	}
}
```

By default, all of the methods above do nothing.

#### Parameterized test

```c++
class Test : public test2::Test2 {
	void run_all() {
		EXECUTE_NAMED(f, "1 - 0.95 < 0.1", 1, 0.95);
		EXECUTE_NAMED(f, "1 - 0.98 < 0.1", 1, 0.98);
	}
	
	void f(int x, double y) {
		double diff = (double)x - y;
		ASSERT2(diff < 0.1);
	}
};
```

Use `EXECUTE_NAMED` to tell apart different executions of the same test.

#### Exceptions

```c++
class MyException : std::exception { ... };

void always_throws() {
	throw MyException();
}

class Test : public test2::Test2() {
	void run_all() {
		EXECUTE_TEST(should_throw_MyException);
	}
	
	void should_throw_MyException() {
		ASSERT2_THROWS(
			MyException, // This is the exception we are expecting the code below will throw.
			{
				// This is the code where MyException should be thrown.
				always_throws();
			},
			{
				// This code runs after MyException is caught.
				// You can do side-effect assertions here.
			}
		);
	}
}
```

There should be at most one `ASSERT2_THROWS` in any test.