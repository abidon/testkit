/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_test_results
#define __testkit_test_results

#include <TestKit/TKTypes.h>

#include <map>
#include <vector>

testkit_namespace_start

/*!
 @class test_results
 @brief
 */
class test_results
{
	std::map<std::string, std::vector<test_result>> _results;
	
public:
	/*!
	 @brief default constructor
	 */
	test_results();
	
	/*!
	 @brief move constructor
	 @param tr the results to move
	 */
	test_results(test_results&& tr);
	
	/*!
	 @brief move assign operator
	 @param tr the results to move
	 */
	test_results& operator=(test_results&&);
	
	/*!
	 @brief add a new test result
	 @param result a test_result instance
	 */
	void
	add_result(const test_result& result);
	
	/*!
	 @brief the suites for which at least one test hasn't passed
	 @returns a vector of tests suites names
	 */
	std::vector<std::string>
	corrupted_suites() const;
	
	/*!
	 @brief the amount of tests which have failed
	 */
	const std::size_t
	failures() const;
	
	/*!
	 @brief the amount of tests which have failed for a specific suite
	 */
	const std::size_t
	failures(const std::string& suite) const;
	
	/*!
	 @brief a list of failed tests results
	 */
	std::vector<test_result>
	failures_list() const;
	
	/*!
	 @brief the amount of tests which have succeeded
	 */
	const std::size_t
	successes() const;
	
	/*!
	 @brief the amount of tests which have succeeded for a specific suite
	 */
	const std::size_t
	successes(const std::string& suite) const;
	
	/*!
	 @brief a list of successful tests results
	 */
	std::vector<test_result>
	successes_list() const;
	
	/*!
	 @brief the tests success rate in percent
	 */
	const float
	success_rate() const;
	
	/*!
	 @brief the tests success rate in percent for a specific suite
	 */
	const std::size_t
	success_rate(const std::string& suite) const;
	
	/*!
	 @brief the amount of tests
	 */
	const std::size_t
	results() const;
	
	/*!
	 @brief the amount of tests for a specific suite
	 */
	const std::size_t
	results(const std::string& suite) const;
	
	/*!
	 @brief the full lists of tests
	 */
	const std::map<std::string, std::vector<tk::test_result>>&
	results_list() const;
	
	/*!
	 @brief the total tests duration in nanoseconds
	 */
	const std::chrono::duration<long long, std::nano>
	total_time() const;
	
	/*!
	 @brief the total tests duration in nanoseconds for a specific suite
	 */
	const std::chrono::duration<long long, std::nano>
	total_time(const std::string& suite) const;
	
private:
	test_results(const test_results&) = delete;
	test_results& operator=(const test_results&) = delete;
};

testkit_namespace_end

#endif
