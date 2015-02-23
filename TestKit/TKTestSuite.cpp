/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#include <TestKit/TKTestSuite.h>

tk::test_suite::test_suite() :
_tests()
{
	
}

tk::test_suite::test_suite(test_suite&& mv) :
_tests(mv._tests)
{
	mv._tests.clear();
}

tk::test_suite::~test_suite()
{ }

tk::test_suite&
tk::test_suite::operator=(test_suite&& mv)
{
	if (&mv == this) return *this;
	
	_tests = mv._tests;
	
	mv._tests.clear();
	
	return *this;
}

void
tk::test_suite::add_test(const std::string& name, test_fn_type fn, bool with_time)
{
	_tests.insert({fn, class_name(), name, suite_name(), with_time});
}

const std::set<tk::test>&
tk::test_suite::tests() const
{
	return _tests;
}
