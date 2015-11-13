/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#include <TestKit/TKHtmlWriter.h>
#include <TestKit/TKTestResults.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string.h>
#include <time.h>

/* HTML tag list:

 charts_time_spent_dataset
 charts_success_rate_dataset
 charts_suites_name
 end_foreach_suite
 end_foreach_test
 foreach_suite
 foreach_test
 results_completion_date
 results_success_rate
 test_suite_max_duration
 test_suite_name
 test_class_name
 test_name
 test_detail
 test_duration
 test_result (pass=success, error=warning, failure=danger)
 testkit_version
 */

namespace priv {
    const std::string default_skeleton = "\n\
    <!DOCTYPE html>\n\
    <!--\n\
    This test report has been generated with TestKit v!!testkit_version\n\
    For more information, visit http://github.com/abidon/testkit\n\
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
    <a class=\"navbar-brand\" href=\"#\">TestKit</a>\n\
    </div>\n\
    </nav>\n\
    \n\
    <div class=\"container-fluid\">\n\
    <div class=\"row\">\n\
    <div class=\"col-sm-12\">\n\
    <div class=\"page-header\">\n\
    <h1>Test report <small>!!results_completion_date</small></h1>\n\
    </div>\n\
    </div>\n\
    </div>\n\
    <div class=\"row\">\n\
    <div class=\"col-sm-4\">\n\
    <canvas id=\"success_per_suite\" width=\"300\" height=\"400\"></canvas>\n\
    </div>\n\
    <div class=\"col-sm-4\">\n\
    <canvas id=\"success_rate\" width=\"300\" height=\"400\"></canvas>\n\
    </div>\n\
    <div class=\"col-sm-4\">\n\
    <canvas id=\"time_per_suite\" width=\"300\" height=\"400\"></canvas>\n\
    </div>\n\
    </div>\n\
    <div class=\"row\">\n\
    <div class=\"col-md-12\">\n\
    <hr/>\n\
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
    <td class=\"!!test_result\" title=\"!!test_detail\"><p>!!test_name</p><small>!!test_detail</small></td>\n\
    <td class=\"!!test_result\">!!test_duration</td>\n\
    </tr>\n\
    !!end_foreach_test\n\
    !!end_foreach_suite\n\
				</table>\n\
    </div>\n\
    </div>\n\
    </div>\n\
    <p style=\"color: #aaa; font-size: 10px; text-align: center;\">Report generated using <a href=\"http://github.com/abidon/testkit\">TestKit</a>, the C++ unit testing framework.</p>\n\
    \n\
    <script src=\"http://www.chartjs.org/assets/Chart.js\"></script>\n\
    <script>\n\
    (function() {\n\
    // Creating 'per suite success rate' chart\n\
    var success_ctx = document.getElementById('success_per_suite').getContext('2d');\n\
    new Chart(success_ctx).Bar({\n\
    labels: [ !!charts_suites_name ],\n\
    datasets: [ !!charts_success_rate_dataset ]\n\
    });\n\
    \n\
    // Creating 'time spent per suite' chart\n\
    var max_duration = Math.ceil(!!test_suite_max_duration);\n\
    var time_spent_ctx = document.getElementById('time_per_suite').getContext('2d');\n\
    new Chart(time_spent_ctx).Line({\n\
    labels: [ !!charts_suites_name ],\n\
    datasets: [ !!charts_time_spent_dataset ]\n\
    }, {scaleOverride: true, scaleSteps: 10, scaleStepWidth: 0.5 * Math.ceil(max_duration/5), scaleStartValue: 0});\n\
    \n\
    var success_rate_ctx = document.getElementById('success_rate').getContext('2d');\n\
    new Chart(success_rate_ctx).Doughnut([\n\
    { color: \"rgba(0,188,0,0.8)\", value: !!results_success_rate },\n\
    { color: \"rgba(188,0,0,0.8)\", value: 100 - !!results_success_rate }\n\
    ]);\n\
    })();\n\
    </script>\n\
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
    srand(time(NULL));

    std::string final_html(_skeleton);
    std::string timestamp(util::timestamp());
    float max_duration = 0.0f;

    final_html = util::replace("!!results_completion_date", timestamp, final_html);
    final_html = util::replace("!!testkit_version", version::literal, final_html);

    {
        std::stringstream ss;
        ss << _r.success_rate();
        final_html = util::replace("!!results_success_rate", ss.str(), final_html);
    }

    final_html = util::replace("!!charts_suites_name", [](const tk::test_results& r) {
        auto results = r.results_list();
        std::string suites("");
        for (auto it(results.begin()); it != results.end(); ++it)
        {
            suites += "\"" + it->first + "\"";

            auto it_copy(it);
            if (++it_copy != results.end())
                suites += ", ";
        }
        return suites;
    }(_r), final_html);

    final_html = util::replace("!!charts_time_spent_dataset", [&max_duration](const tk::test_results& r) {
        auto results = r.results_list();
        std::stringstream output;

        std::array<std::vector<double>, 2> datasets_data;

        for (auto it(results.begin()); it != results.end(); it++)
        {
            double sum = 0;
            for (auto rit(it->second.begin()); rit != it->second.end(); ++rit)
                sum += rit->ns.count() / 1000000000.f;
            datasets_data[0].push_back(sum);
            datasets_data[1].push_back(sum / it->second.size());
            if (sum > max_duration)
                max_duration = sum;
        }

        for (uint64_t j(0); j < datasets_data.size(); ++j)
        {
            output << "{data: [";

            for (uint64_t i(0); i < datasets_data[j].size(); ++i)
                output << datasets_data[j][i] << (i < datasets_data[j].size()-1 ? "," : "");

            output << "],";
            uint8_t r = (j == 0 ? 20 : 82),
                g = (j == 0 ? 220 : 126),
                b = (j == 0 ? 228 : 230);
            output << "fillColor: \"rgba(" << (int)r << "," << (int)g << "," << (int)b << ",0.25)\",";
            output << "pointColor: \"rgba(" << (int)r << "," << (int)g << "," << (int)b << ",1)\",";
            output << "strokeColor: \"rgba(" << (int)r << "," << (int)g << "," << (int)b << ",1)\"";

            output << "}";

            if (j < datasets_data.size()-1)
                output << ",\n";
        }

        return output.str();
    }(_r), final_html);

    {
        std::stringstream ss;
        ss << max_duration;
        final_html = util::replace("!!test_suite_max_duration", ss.str(), final_html);
    }

    final_html = util::replace("!!charts_success_rate_dataset", [](const tk::test_results& r) {
        auto results = r.results_list();
        std::stringstream output;

        std::array<std::vector<uint64_t>, 2> datasets_data;

        for (auto suite_it(results.begin()); suite_it != results.end(); ++suite_it)
        {
            datasets_data[0].push_back(r.successes(suite_it->first));
            datasets_data[1].push_back(r.results(suite_it->first) - r.successes(suite_it->first));
        }



        for (uint64_t j(0); j <= datasets_data.size(); ++j)
        {
            output << "{data: [";

            for (uint64_t i(0); i < datasets_data[j > 0 ? j-1 : j].size(); ++i)
                if (j == 0)
                    output << (datasets_data[0][i] + datasets_data[1][i]) << (i < datasets_data[j].size()-1 ? "," : "");
                else output << datasets_data[j-1][i] << (i < datasets_data[j-1].size()-1 ? "," : "");

            output << "],";

            std::string color = j == 0 ? "30,40,188" : (j == 1 ? "0,188,0" : "188,0,0");
            output << "fillColor: \"rgba(" + color + ",0.25)\",";
            output << "pointColor: \"rgba(" + color + ",1)\",";
            output << "strokeColor: \"rgba(" + color + ",1)\"";

            output << "}";

            if (j < datasets_data.size())
                output << ",\n";
        }

        return output.str();
    }(_r), final_html);

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
                    test_html = util::replace("!!test_detail", test_it->details, test_html);

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
