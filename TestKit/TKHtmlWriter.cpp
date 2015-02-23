/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#include <TestKit/TKHtmlWriter.h>
#include <TestKit/TKTestResults.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <time.h>

/* HTML tag list:
 
 end_foreach_suite
 end_foreach_test
 foreach_suite
 foreach_test
 results_completion_date
 test_suite_name
 test_class_name
 test_name
 test_duration
 test_result (pass=success, error=warning, failure=danger)
 testkit_version
 */

namespace priv {
	const std::string default_skeleton = "\n\
<!DOCTYPE html>\n\
<!--\n\
	This test report has been generated with TestKit v!!testkit_version\n\
	For more information, visit http://developers.boltstud.io/testkit\n\
-->\n\
<html>\n\
<head>\n\
	<meta charset=\"utf-8\">\n\
	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\
	<title>Test Report — !!results_completion_date</title>\n\
	\n\
	<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css\">\n\
</head>\n\
<body>\n\
	<nav class=\"navbar navbar-inverse navbar-static-top\" role=\"navigation\">\n\
		<div class=\"container-fluid\">\n\
			<a class=\"navbar-brand\" href=\"#\">Test Report <small>!!results_completion_date</small></a>\n\
		</div>\n\
	</nav>\n\
	\n\
	<div class=\"container-fluid\">\n\
		<div class=\"row\">\n\
			<canvas id=\"chart\" width=\"100%\" height=\"100%\"></canvas>\n\
		</div>\n\
		<div class=\"row\">\n\
			<div class=\"col-md-12\">\n\
				<table class=\"table table-hover\">\n\
					<tr>\n\
						<th>Class</th>\n\
						<th>Name</th>\n\
						<th>Duration</th>\n\
					</tr>\n\
					!!foreach_suite\n\
					<tr class=\"info\">\n\
						<th colspan=\"3\" scope=\"rowgroup\">suite !!test_suite_name</th>\n\
					</tr>\n\
					!!foreach_test\n\
					<tr>\n\
						<td>!!test_class_name</td>\n\
						<td class=\"!!test_result\">!!test_name</td>\n\
						<td>!!test_duration</td>\n\
					</tr>\n\
					!!end_foreach_test\n\
					!!end_foreach_suite\n\
				</table>\n\
			</div>\n\
		</div>\n\
	</div>\n\
	\n\
	<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js\"></script>\n\
</body>\n\
</html>";
}

namespace util {
	std::string timestamp()
	{
		time_t t = time(0);
		struct tm* lt = localtime(&t);
		std::stringstream ss;
		ss << std::setw(2) << std::setfill('0') << lt->tm_mday << "/" << std::setw(2) << std::setfill('0') << lt->tm_mon+1 << "/" << 1900+lt->tm_year << " " << std::setw(2) << std::setfill('0') << lt->tm_hour << ":" << std::setw(2) << std::setfill('0') << lt->tm_min << ":" << std::setw(2) << std::setfill('0') << lt->tm_sec;
		return ss.str();
	}
	
	std::string replace(const std::string& pattern, const std::string& replace, const std::string& input)
	{
		std::string wc(input);
		
		std::size_t pos = wc.find(pattern);
		while (pos != std::string::npos)
		{
			wc.replace(pos, pattern.size(), replace, 0, replace.size());
			pos = wc.find(pattern);
		}
		
		return wc;
	}
}

tk::html_writer::html_writer(const test_results& r) :
_r(r),
_skeleton(priv::default_skeleton)
{
	
}

void
tk::html_writer::write(std::ostream &s) const
{
	std::string final_html(_skeleton);
	std::string timestamp(util::timestamp());
	
	final_html = util::replace("!!results_completion_date", timestamp, final_html);
	final_html = util::replace("!!testkit_version", version::literal, final_html);
	
	// for each suite
	std::size_t fes_beg = final_html.find("!!foreach_suite");
	std::size_t fes_end = final_html.find("!!end_foreach_suite");
	auto res_list = _r.results_list();
	while (fes_beg != std::string::npos && fes_end != std::string::npos)
	{
		while (fes_end < fes_beg && fes_end != std::string::npos)
		{
			fes_end = final_html.find("!!end_foreach_suite", fes_end+1);
		}
		if (fes_end == std::string::npos) break;
		
		const std::string suite_template = final_html.substr(fes_beg+strlen("!!foreach_suite"), (fes_end-fes_beg)-strlen("!!foreach_suite"));
		final_html.replace(fes_beg, (fes_end-fes_beg)+strlen("!!end_foreach_suite"), "");
		
		std::size_t insert_pos = fes_beg;
		for (auto it = res_list.begin(); it != res_list.end(); it++)
		{
			std::string suite_html = util::replace("!!test_suite_name", it->first, suite_template);
			
			// for each test
			std::size_t fet_beg = suite_html.find("!!foreach_test");
			std::size_t fet_end = suite_html.find("!!end_foreach_test");
			std::vector<tk::test_result> tests = it->second;
			while (fet_beg != std::string::npos && fet_end != std::string::npos)
			{
				while (fes_end < fes_beg && fes_end != std::string::npos)
				{
					fet_end = suite_html.find("!!end_foreach_test", fet_end+1);
				}
				if (fet_end == std::string::npos) break;
				
				const std::string test_template = suite_html.substr(fet_beg+strlen("!!foreach_test"), (fet_end-fet_beg)-strlen("!!foreach_test"));
				suite_html.replace(fet_beg, (fet_end-fet_beg)+strlen("!!end_foreach_test"), "");
				
				std::size_t test_insert_pos = fet_beg;
				for (auto test_it = tests.begin(); test_it != tests.end(); test_it++) {
					std::string test_html = test_template;
					
					test_html = util::replace("!!test_class_name", test_it->test.clazz, test_html);
					test_html = util::replace("!!test_name", test_it->test.name, test_html);
					test_html = util::replace("!!test_result", test_it->passed ? "success" : "danger", test_html);
					
					if (test_it->test.with_timer && test_it->passed)
					{
						std::stringstream ss;
						unsigned long long ns = test_it->ns.count();
						if (ns < 1000)
							ss << ns << "ns";
						else if (ns < 1000000)
							ss << ns/1000.f << "µs";
						else if (ns < 1000000000)
							ss << ns/1000000.f << "ms";
						else ss << ns/1000000000.f << "sec";
						test_html = util::replace("!!test_duration", ss.str(), test_html);
					} else {
						test_html = util::replace("!!test_duration", "untimed", test_html);
					}
					
					suite_html.insert(test_insert_pos, test_html);
					test_insert_pos += test_html.length();
				}
				
				fet_beg = suite_html.find("!!foreach_test");
				fet_end = suite_html.find("!!end_foreach_test");
			}
			
			final_html.insert(insert_pos, suite_html);
			insert_pos += suite_html.length();
		}
		
		fes_beg = final_html.find("!!foreach_suite");
		fes_end = final_html.find("!!end_foreach_suite");
	}
	
	s << final_html;
}
