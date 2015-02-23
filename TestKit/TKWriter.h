/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_writer
#define __testkit_writer

#include <TestKit/TKTypes.h>
#include <iostream>

testkit_namespace_start

struct writer
{
	/*!
	 @brief write to stream
	 @param s the stream to write the result too
	 */
	virtual void
	write(std::ostream& s) const pure;
};

testkit_namespace_end

#endif
