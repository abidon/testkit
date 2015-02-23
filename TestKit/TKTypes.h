/*
 * TestKit
 * The unit testing framework that makes you gain productivity
 * Copyright (c) 2014 Aurelien Bidon
 */

#ifndef __testkit_types
#define __testkit_types

#include <chrono>

#define testkit_namespace_start	namespace tk {
#define testkit_namespace_end	}

#include <TestKit/TKVersion.h>

testkit_namespace_start

// defines and helpers
#define pure = 0
typedef std::chrono::duration<long long, std::nano> nano_duration;

// forward declarations
// - without dependencies
class junit_writer;
class test_runner;
class test_results;
class text_writer;

// - with dependencies

testkit_namespace_end

// data structs
#include <TestKit/TKTest.h>

// interfaces
#include <TestKit/TKTestSuite.h>
#include <TestKit/TKWriter.h>

#endif