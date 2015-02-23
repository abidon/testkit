/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#include "TKMechanisms.h"

#include <sstream>

tk::test_fail::test_fail(const char* msg) :
reason(msg)
{ }

tk::test_fail::test_fail(const std::string& msg) :
reason(msg)
{ }

tk::test_fail::test_fail(const tk::test_fail& cp) :
reason(cp.reason)
{ }

tk::test_fail::test_fail(const char* exp, const char* msg, const char* file, const std::size_t line)
{
	std::stringstream ss;
	ss << "Failure at " << file << ":" << line << ": { " << exp << " } " << msg;
	reason = ss.str();
}

tk::test_fail::test_fail(const char* exp, const std::string& msg, const char* file, const std::size_t line)
{
	std::stringstream ss;
	ss << "Failure at " << file << ":" << line << ": { " << exp << " } " << msg;
	reason = ss.str();
}

tk::test_fail::test_fail(const char* exp, const char* msg, const char* exc_t, const char* file, const std::size_t line)
{
	std::stringstream ss;
	ss << "Failure at " << file << ":" << line << ": { " << exp << " } " << msg << " " << exc_t;
	reason = ss.str();
}

tk::test_fail::test_fail(const char* exp, const std::string& msg, const char* exc_t, const char* file, const std::size_t line)
{
	std::stringstream ss;
	ss << "Failure at " << file << ":" << line << ": { " << exp << " } " << msg << " " << exc_t;
	reason = ss.str();
}

const char*
tk::test_fail::what() const noexcept
{
	return reason.c_str();
}

void
tk::assert(const bool exp, const char* code, const char* file, const std::size_t line)
{
	if (!exp) {
		std::stringstream ss;
		ss << "Assertion failed at " << file << ":" << line << " { " << code << " }";
		throw tk::test_fail(ss.str());
	}
}
