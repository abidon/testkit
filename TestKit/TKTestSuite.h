/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_test_suite
#define __testkit_test_suite

#include <TestKit/TKTypes.h>
#include <set>
#include <string>

testkit_namespace_start

class test_suite
{
	std::set<test> _tests;
	
public:
	test_suite();
	test_suite(const test_suite&) = delete;
	test_suite(test_suite&&);
	
	virtual ~test_suite() = 0;
	
	test_suite& operator=(const test_suite&) = delete;
	test_suite& operator=(test_suite&&);
	
	void
	add_test(const std::string& name, test_fn_type fn, bool with_time = true);
	
	virtual const std::string
	class_name() const pure;
	
	virtual const std::string
	suite_name() const pure;
	
	const std::set<test>&
	tests() const;
};

testkit_namespace_end

#define TK_DECLARE_SUITE(suite_name) class suite_name : public tk::test_suite
#define TK_SET_SUITE_NAME(suite_name) virtual const std::string suite_name() const { return suite_name; }
#define TK_SET_TARGET_CLASS(class_name) virtual const std::string class_name() const { return class_name; }

#endif
