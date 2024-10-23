#pragma once
#include "MintyLib.h"

namespace MintyTest
{
	struct Result
	{
		Minty::UInt passed;
		Minty::UInt total;
	};
	
	// Tests by file
	Result test_application();

	// All tests
	Result test_all();
}