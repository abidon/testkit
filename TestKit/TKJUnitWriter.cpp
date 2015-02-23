/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#include <TestKit/TKJunitWriter.h>
#include <TestKit/TKTestResults.h>
#include <time.h>

tk::junit_writer::junit_writer(const test_results& r) :
_r(r)
{
	
}

void
tk::junit_writer::write(std::ostream& os) const
{
	os << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << std::endl;
	os << "<testsuites>" << std::endl;
	
	const std::streamsize previous_precision = os.precision();
	os.precision(9);
	
	for (const std::pair<std::string, std::vector<test_result>>& suite : _r.results_list())
	{
		os << "\t<testsuite failures=\"" << _r.failures(suite.first) << "\" tests=\"" << _r.results(suite.first) << "\" errors=\"0\" name=\"" << suite.first << "\">" << std::endl;
		for (test_result tr : suite.second)
		{
			if (true == tr.passed)
			{
				os << "\t\t<testcase classname=\"" << tr.test.clazz << "\" name=\"" << tr.test.name << "\" time=\"" << std::fixed << tr.ns.count()/1000000000.f << "\"/>" << std::endl;
			} else {
				os << "\t\t<testcase classname=\"" << tr.test.clazz << "\" name=\"" << tr.test.name << "\" time=\"" << std::fixed << tr.ns.count()/1000000000.f << "\">" << std::endl;
				os << "\t\t\t<failure type=\"testkit.assertion_failure\">" << tr.details << "</failure>" << std::endl;
				os << "\t\t</testcase>" << std::endl;
			}
		}
		os << "\t</testsuite>" << std::endl;
	}
		
	os << "</testsuites>";
	
	os.precision(previous_precision);
}
