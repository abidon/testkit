/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_version
#define __testkit_version

testkit_namespace_start

struct version {
	static const unsigned major = 0;
	static const unsigned minor = 2;
	static const unsigned patch = 0;
	static constexpr const char* literal = "0.2.0";
};

testkit_namespace_end

#endif