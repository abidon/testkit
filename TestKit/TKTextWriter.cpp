/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#include <TestKit/TKTextWriter.h>
#include <TestKit/TKTestResults.h>
#include <iomanip>
#include <time.h>

tk::text_writer::text_writer(const test_results& r) :
_r(r)
{
	
}

void
tk::text_writer::write(std::ostream& os) const
{
	time_t t = time(0);
	struct tm* lt = localtime(&t);
	
	// Date/time of printing
	os << "Tests Results — ended at " << std::setw(2) << std::setfill('0') << lt->tm_mday << "/" << std::setw(2) << std::setfill('0') << lt->tm_mon+1 << "/" << 1900+lt->tm_year << " " << std::setw(2) << std::setfill('0') << lt->tm_hour << ":" << std::setw(2) << std::setfill('0') << lt->tm_min << ":" << std::setw(2) << std::setfill('0') << lt->tm_sec << std::endl;
	os << std::endl;
	
	// Quick summary
	os << "— Quick summary: " << std::endl;
	{
		os << "\tTotal duration: ";
		unsigned long long ns = _r.total_time().count();
		if (ns < 1000)
			os << ns << "ns";
		else if (ns < 1000000)
			os << ns/1000.f << "µs";
		else if (ns < 1000000000)
			os << ns/1000000.f << "ms";
		else os << ns/1000000000.f << "sec";
		os << std::endl;
	}
	os << "\tTest(s): " << _r.results() << std::endl;
	os << "\tSuccess(es): " << _r.successes() << std::endl;
	os << "\tFailure(s): " << _r.failures() << std::endl;
	os << "\tSuccess rate: " << _r.success_rate() << std::endl;
	
	os << "\tCorrupted Suite(s): ";
	if (_r.corrupted_suites().size() == 0) os << "none" << std::endl;
	else os << std::endl;
	for (std::string sn : _r.corrupted_suites())
		os << "\t\t— " << sn << std::endl;
	
	// Successes
	if (_r.successes() > 0)
	{
		os << "Successful test(s):" << std::endl;
		std::vector<test_result> successes = _r.successes_list();
		for (test_result tr : successes)
		{
			os << "\t— " << tr.test.suite << "::" << tr.test.name;
			if (tr.test.with_timer)
			{
				unsigned long long ns = tr.ns.count();
				if (ns < 1000)
					os << ": passed in " << ns << "ns";
				else if (ns < 1000000)
					os << ": passed in " << ns/1000.f << "µs";
				else if (ns < 1000000000)
					os << ": passed in " << ns/1000000.f << "ms";
				else os << ": passed in " << ns/1000000000.f << "sec";
			}
			os << std::endl;
		}
	}
	
	// Failures
	if (_r.failures() > 0)
	{
		os << "Failed test(s):" << std::endl;
		std::vector<test_result> fails = _r.failures_list();
		for (test_result tr : fails)
		{
			os << "\t— " << tr.test.suite << "::" << tr.test.name << ": " << tr.details << std::endl;
		}
	}
}
