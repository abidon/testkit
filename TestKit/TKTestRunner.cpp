/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#include <TestKit/TKTestResults.h>
#include <TestKit/TKTestRunner.h>
#include <TestKit/TKTestSuite.h>
#include <iostream>

tk::test_runner::test_runner() :
_suites(),
_results(new tk::test_results())
{ }

tk::test_runner::test_runner(test_runner&& mv) :
_suites(mv._suites),
_results(mv._results)
{
	mv._results = new tk::test_results();
	mv._suites.clear();
}

tk::test_runner::~test_runner()
{
	for (test_suite* t : _suites)
		delete t;
	delete _results;
}

tk::test_runner&
tk::test_runner::operator=(test_runner&& mv)
{
	if (&mv == this) return *this;
	
	_suites = mv._suites;
	mv._suites.clear();
	
	return *this;
}

void
tk::test_runner::add_suite(tk::test_suite* suite)
{
	if (suite == nullptr) return;
	_suites.insert(suite);
}

void
tk::test_runner::run()
{
	for (test_suite* ts : _suites)
	{
		const std::set<tk::test>& tests = ts->tests();
		for (const tk::test& test : tests)
		{
			auto start_time = std::chrono::high_resolution_clock::now();
			try {
				test.fn();
			} catch (std::exception& e) {
				std::clog << "F";
				_results->add_result((tk::test_result){test, false, std::chrono::nanoseconds(0), e.what()});
				continue;
			} catch (...)
			{
				std::clog << "E";
				_results->add_result((tk::test_result){test, false, std::chrono::nanoseconds(0), "unknown error (test has thrown something not an std::exception)"});
				continue;
			}
			auto end_time = std::chrono::high_resolution_clock::now();
			std::clog << ".";
			_results->add_result((tk::test_result){test, true, std::chrono::nanoseconds(end_time-start_time), ""});
		}
	}
	std::clog << std::endl;
}

const tk::test_results&
tk::test_runner::results() const
{
	return *_results;
}
