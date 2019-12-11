#pragma once

#include <string>
#include <map>

#include "error_numbers.h"

namespace meh {

	using debug_messages_t = std::map<error_number_t, std::string>;

	static debug_messages_t debug_messages = {
	{DNOCONVERSION, "unrecognised token"},
	{DOUTRANGE, "converted value would fall out of the range of the result type"},
	{DNOARGS, "not enough arguements on stack"}
	};

}

/*
const size_t DNOCONVERSION = 1;
	const size_t DOUTRANGE = 2;
	const size_t DNOARGS = 3;
	const size_t DNOQUOTE = 4;
	const size_t DMALSTACK = 5;
	const size_t DUNDERFLOW = 6;
}
*/
