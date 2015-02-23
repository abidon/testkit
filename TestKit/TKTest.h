/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_test
#define __testkit_test

#include <TestKit/TKTypes.h>
#include <string>
#include <utility>

testkit_namespace_start

typedef void(*test_fn_type)(void);

/*!
 @struct test
 @brief holds information about a test
 */
struct test
{
	/*!
	 @brief a pointer to the test function
	 */
	test_fn_type fn;
	
	/*!
	 @brief the class targeted by the test
	 */
	std::string clazz;
	
	/*!
	 @brief the name of the test
	 */
	std::string name;
	
	/*!
	 @brief the test suite the test comes from
	 */
	std::string suite;
	
	/*!
	 @brief should the test duration be stored
	 */
	bool with_timer;
	
	bool
	operator<(const test& t) const
	{
		return clazz < t.clazz || name < t.name;
	}
};

/*!
 @struct test_result
 @brief holds information about the result of a test
 */
struct test_result
{
	/*!
	 @brief the test associated with the result
	 */
	test test;
	
	/*!
	 @brief has the test passed
	 */
	bool passed;
	
	/*!
	 @brief the duration of the test (0 if test.with_timer == false)
	 */
	nano_duration ns;
	
	/*!
	 @brief details about the test result (empty string if the test passed)
	 */
	std::string details;
};

testkit_namespace_end

#endif
