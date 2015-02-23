/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_junit_writer
#define __testkit_junit_writer

#include <TestKit/TKTypes.h>

testkit_namespace_start

/*!
 @class junit_writer
 @brief JUnit-compatible results writring class
 */
class junit_writer :
public writer
{
	const test_results& _r;
	
public:
	/*!
	 @brief constructor from results
	 @param r the results to write
	 */
	junit_writer(const test_results& r);
	
	/*!
	 @brief write to stream
	 @param s the stream to write the result too
	 */
	void
	write(std::ostream& s) const;
};

testkit_namespace_end

#endif
