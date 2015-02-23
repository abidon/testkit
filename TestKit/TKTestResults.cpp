/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#include <TestKit/TKTestResults.h>

tk::test_results::test_results() :
_results()
{ }

tk::test_results::test_results(test_results&& mv) :
_results(mv._results)
{
	mv._results.clear();
}

tk::test_results&
tk::test_results::operator=(test_results&& mv)
{
	if (&mv == this) return *this;
	
	_results = mv._results;
	mv._results.clear();
	
	return *this;
}

void
tk::test_results::add_result(const tk::test_result& result)
{
	_results[result.test.suite].push_back(result);
}

std::vector<std::string>
tk::test_results::corrupted_suites() const
{
	std::vector<std::string> suites;
	
	for (const std::pair<std::string, std::vector<test_result>>& suite : _results)
	{
		for (const test_result& tr : suite.second)
		{
			if (false == tr.passed)
			{
				suites.push_back(suite.first);
				continue;
			}
		}
	}
	
	return suites;
}

const std::size_t
tk::test_results::failures() const
{
	return results() - successes();
}

const std::size_t
tk::test_results::failures(const std::string& suite) const
{
	return results(suite) - successes(suite);
}

std::vector<tk::test_result>
tk::test_results::failures_list() const
{
	std::vector<tk::test_result> l;
	for (const std::pair<std::string, std::vector<test_result>>& suite : _results)
	{
		for (const test_result& tr : suite.second)
		{
			if (false == tr.passed)
				l.push_back(tr);
		}
	}
	return l;
}

const std::size_t
tk::test_results::successes() const
{
	std::size_t count = 0;
	for (const std::pair<std::string, std::vector<test_result>>& suite : _results)
	{
		for (test_result tr : suite.second)
		{
			count += tr.passed ? 1 : 0;
		}
	}
	return count;
}

const std::size_t
tk::test_results::successes(const std::string& suitename) const
{
	if (suitename == "") return successes();
	std::size_t count = 0;
	
	try {
		const std::vector<test_result>& suite = _results.at(suitename);
		
		for (test_result tr : suite)
		{
			count += tr.passed ? 1 : 0;
		}
	} catch (...) {
		return 0;
	}
	
	return count;
}

std::vector<tk::test_result>
tk::test_results::successes_list() const
{
	std::vector<tk::test_result> l;
	for (const std::pair<std::string, std::vector<test_result>>& suite : _results)
	{
		for (const test_result& tr : suite.second)
		{
			if (true == tr.passed)
				l.push_back(tr);
		}
	}
	return l;
}

const float
tk::test_results::success_rate() const
{
	const float s = successes();
	const float t = results();
	if (results() == 0) return 0;
	
	return s/t * 100.f;
}

const std::size_t
tk::test_results::success_rate(const std::string& suite) const
{
	const float s = successes(suite);
	const float t = results(suite);
	if (results() == 0) return 0;
	
	return s/t * 100.f;
}

const std::size_t
tk::test_results::results() const
{
	std::size_t res = 0;
	
	for (auto a : _results)
	{
		res += a.second.size();
	}
	
	return res;
}

const std::size_t
tk::test_results::results(const std::string& suite) const
{
	try {
		return _results.at(suite).size();
	} catch (...) {
		return 0;
	}
}

const std::map<std::string, std::vector<tk::test_result>>&
tk::test_results::results_list() const
{
	return _results;
}

const std::chrono::duration<long long, std::nano>
tk::test_results::total_time() const
{
	std::chrono::duration<long long, std::nano> time(0);
	for (const std::pair<std::string, std::vector<test_result>>& suite : _results)
	{
		for (test_result tr : suite.second)
		{
			time+=tr.ns;
		}
	}
	return time;
}

const std::chrono::duration<long long, std::nano>
tk::test_results::total_time(const std::string& suitename) const
{
	if (suitename == "") return std::chrono::seconds(0);
	std::chrono::duration<long long, std::nano> time(0);
	
	try {
		const std::vector<test_result>& suite = _results.at(suitename);
		
		for (test_result tr : suite)
		{
			time+=tr.ns;
		}
	} catch (...) {
		return std::chrono::seconds(0);
	}
	
	return time;
}
