/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_mechanisms
#define __testkit_mechanisms

#include <TestKit/TKTypes.h>
#include <exception>

testkit_namespace_start

/*!
 @struct test_fail
 @brief exception subclass containing informations about a test fail
 */
struct test_fail : std::exception
{
	std::string reason;

	test_fail(const char*);
	test_fail(const std::string&);
	test_fail(const char*, const char*, const char*, const std::size_t);
	test_fail(const char*, const std::string&, const char*, const std::size_t);
	test_fail(const char*, const char*, const char*, const char*, const std::size_t);
	test_fail(const char*, const std::string&, const char*, const char*, const std::size_t);
	test_fail(const test_fail&);

	virtual const char*
	what() const noexcept;
};

#define tk_assert(exp) tk::test_assert(exp, #exp, __FILE__, __LINE__)
#define tk_test_assert(exp) tk::test_assert(exp, #exp, __FILE__, __LINE__) //! @deprecated
#define tk_test_throw(exp, exc_t) {bool nt = false; try {exp;nt=true;}catch(exc_t){}catch(...){throw tk::test_fail(#exp, "have thrown something else than", #exc_t, __FILE__, __LINE__);} if(nt){throw tk::test_fail(#exp, "should have thrown", #exc_t, __FILE__, __LINE__);}}
#define tk_test_no_throw(exp) try{exp;}catch(...){throw tk::test_fail(#exp, "shouldn't have thrown", __FILE__, __LINE__);}

void
test_assert(const bool exp, const char* code, const char* file, const std::size_t line);

#define TK_ADD_TEST(t) add_test(#t, t)
#define TK_DECLARE_TEST(t) static void t()
#define TK_IMPLEMENT_TEST(s, t) void s::t()

testkit_namespace_end

#endif
