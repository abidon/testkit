/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_textwriter
#define __testkit_textwriter

#include <TestKit/TKTypes.h>

testkit_namespace_start

/*!
 @class text_writer
 @brief textual/human readable results writing class
 */
class text_writer :
public writer
{
	const test_results& _r;
	
public:
	/*!
	 @brief constructor from results
	 @param r the results to write
	 */
	text_writer(const test_results&);
	
	/*!
	 @brief write to stream
	 @param s the stream to write the result too
	 */
	void
	write(std::ostream& s) const;
};

testkit_namespace_end

#endif
