#pragma once

#include <string>
#include <map>

#include "error_numbers.h"

namespace joy {

	using debug_messages_t = std::map<error_number_t, std::string>;

	static debug_messages_t debug_messages = {
	{DNOSTATE, "unrecognised state"},
	{DNOCONVERSION, "unrecognised token"},
	{DOUTRANGE, "converted value would fall out of the range of the result type"},
	{DNOARGS, "not enough arguements on stack"},
	{DNOQUOTE, "not a quoted program or list"},
	{DMALSTACK, "malformed stack - missing"},
	{DUNDERFLOW, "stack underflow"},
	{DEMPTYLINE, "no parse empty line"},
	{DNOTSTROPPING, "ignored ] no matching [ "},
	{DNOTDEFINING, "expected =="},
	{DATOMEXISTS, "atom already exists"}
	};

}

