The TestKit Framework Overview
---

* Introduction
* Features
* Getting Started

## Introduction

TestKit is a unit testing framework written in and for the C++ language. It is intented for every kind of project size with an easy to learn/use API. It has no dependencies, except the C++ standard library and have a cool set of — always growing — features.

## Features

TestKit expose a simple but powerful API to common testing mechanisms. Create classes which inherits from the `test_suite` interface, start writing your tests into theses, create a `test_runner` instance, register your test suites into it, choose one or more exporting formats, run, and it works!

TestKit goal is to provide an easy to use interface, but it doesn't mean that it's not powerful. TestKit allows you to export the tests results in several formats: _JUnit XML_, _HTML_, _Textual/Human readable_. And it's easy for you to support new formats.

## Getting Started

### Writing the test running function

First, take a look at the typical `main()` function structure:

```c++
#include <fstream>
#include <iostream>
#include <TestKit/TestKit.h>
#include "my_first_test_suite.h"
#include "my_second_test_suite.h"

int main(int argc, const char** argv)
{
	tk::test_runner runner; 									// (1)
	
	runner.add_suite(new my_first_test_suite); 					// (2)
	runner.add_suite(new my_second_test_suite);
	
	runner.run(); 												// (3)
	
	tk::text_writer(runner.results()).write(std::clog); 		// (4)
	
	std::ofstream junit_file("results.xml"); 					// (5)
	tk::junit_writer(runner.results()).write(junit_file);
	junit_file.flush();
	junit_file.close();
}
```

1. Create a `test_runner` instance, responsible of running the test (you said “_obvious_”?)
2. Register your test suites classes. `add_suite` expect for a `test_suite*` parameter, so anything inheriting from `test_suite` will work. The `test_runner` is responsible of the memory management of the test suites, destroying theses ones for you.
3. Run all the registered tests.
4. You can write the results in a textual form with the `text_writer` class. You can expect the following result:

		Tests Results — ended at 01/09/2014 14:07:57
		
		— Quick summary: 
			Total duration: 4µs
			Test(s): 3
			Success(es): 2
			Failure(s): 1
			Success rate: 66.666667
			Corrupted Suite(s): my_first_test_suite
		Successful test(s):
			— my_first_test_suite::test_two: passed in 1.647µs
			— my_second_test_suite::the_test: passed in 1.214µs
		Failed test(s):
			— my_first_test_suite::test_one: Assertion failed at my_first_test_suite.h:20: { true == false }

5. You can also write the results in the JUnit format, in order to integrate your tests is a building factory like [Bamboo](https://www.atlassian.com/software/bamboo)

	```xml
	<?xml version="1.0" encoding="UTF-8" ?>
	<testsuites>
		<testsuite failures="1" tests="2" errors="0" name="my_first_test_suite">
			<testcase classname="bk::my_first_class" name="test_one" time="0">
				<failure>Assertion failed at my_first_test_suite.h:20: { true == false }</failure>
			</testcase>
			<testcase classname="my_first_class" name="test_two" time="0.000001647"/>
		</testsuite>
		<testsuite failures="0" tests="1" errors="0" name="my_second_test_suite">
			<testcase classname="my_second_class" name="the_test" time="0.000001214"/>
		</testsuite>
	</testsuites>
	```

Now you know how to run your test suites, we can take a look on how to write one

### 3.2 Writing a test suite

In TestKit, test suites are classes that must inherits the `tk::test_suite` interface. To make it easier, you can declare the class using the `TK_DECLARE_SUITE(suite_name)` macro.  
`tk::test_suite` provides two virtual pure methods that you must implement:

- `virtual const std::string class_name() const`: simply return the name of the class the test suite is targeting
	- you can also use the helper macro `TK_SET_TARGET_CLASS("class_name")`
- `virtual const std::string suite_name() const`: simply return the name of the test suite class
	- you can also use the helper macro `TK_SET_SUITE_NAME("suite_name")`

The useful — not to say vital — method you will use to add the tests to the suite is `add_test`, or the `TK_ADD_TEST` macro.  
But we talked too much, let's take a look at our `my_first_test_suite` class:

```c++
TK_DECLARE_SUITE(my_first_test_suite)
{
public:
	my_first_test_suite()
	{
		add_test("test_one", test_one);
		TK_ADD_TEST(test_two);
	}
	
	TK_SET_TARGET_NAME("my_first_class");
	TK_SET_SUITE_NAME("my_first_test_suite");
	
private:
	// The first way to declare a test in a suite
	static void
	test_one()
	{
		tk_test_assert(true == false); // this test will intentionally fail
	}
	
	// The other way to declare a test in a suite
	TK_DECLARE_TEST(test_two)
	{
		tk_test_assert(3*3 == 9);
	}
}
```

A test must be static, can't have parameters nor return a value.

Here's a non exhaustive list of the testing mechanisms TestKit offers:

- `tk_test_assert(expression)` or `tk_assert(expression)`: the test fail is the expression isn't `true`
- `tk_test_no_throw(expression)`: ensure an expression doesn't throw any kind of exception
- `tk_test_throw(expression, exception_name)`: ensure an expression throw a specific kind of exception

# License

TestKit is released under the MIT License (MIT)

Copyright (c) 2015 Aurélien Bidon

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
