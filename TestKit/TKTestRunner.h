/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_test_runner
#define __testkit_test_runner

#include <TestKit/TKTypes.h>
#include <set>

testkit_namespace_start

/*!
 @class test_runner
 */
class test_runner
{
	std::set<tk::test_suite*> _suites;
	tk::test_results* _results;
	
public:
	/*!
	@brief default constructor
	 */
	test_runner();
	
	/*!
	 @brief move constructor
	 @param tr the test_runner to move
	 */
	test_runner(test_runner&& tr);
	
	/*!
	 @brief move assignation operator
	 @param tr the test_runner to move
	 */
	test_runner& operator=(test_runner&& tr);
	
	/*!
	 @brief destructor
	 */
	~test_runner();
	
	/*!
	 @brief add a test suite to the runner
	 @param suite the suite to run (will be destroyed by the runner itself)
	 */
	void
	add_suite(tk::test_suite* suite);
	
	/*!
	 @brief run all the associated tests suites
	 */
	void
	run();
	
	/*!
	 @brief get the results of the tests
	 @returns a test_results instance, empty if the run method hasn't been called, or if no tests have been associated.
	 */
	const tk::test_results&
	results() const;
	
private:
	test_runner(const test_runner&) = delete;
	test_runner& operator=(const test_runner&) = delete;
};

testkit_namespace_end

#endif