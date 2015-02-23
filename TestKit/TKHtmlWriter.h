/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_html_writer
#define __testkit_html_writer

#include <TestKit/TKTypes.h>

testkit_namespace_start

/*!
 @class html_writer
 @brief fancy HTML results writring class
 */
class html_writer :
public writer
{
	const test_results& _r;
	const std::string _skeleton;
	
public:
	/*!
	 @brief constructor from results
	 @param r the results to write
	 */
	html_writer(const test_results& r);
	
	/*!
	 @brief write to stream
	 @param s the stream to write the result too
	 */
	void
	write(std::ostream& s) const;
};

testkit_namespace_end

#endif