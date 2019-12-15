#pragma once

/**
* (D)ebug error numbers
*/

namespace joy {

	using error_number_t = size_t;

	const size_t DNOSTATE = 0;
	const size_t DNOCONVERSION = 1;
	const size_t DOUTRANGE = 2;
	const size_t DNOARGS = 3;
	const size_t DNOQUOTE = 4;
	const size_t DMALSTACK = 5;
	const size_t DUNDERFLOW = 6;
	const size_t DEMPTYLINE = 7;
	const size_t DNOTSTROPPING = 8;
	const size_t DNOTDEFINING = 9;
	const size_t DATOMEXISTS = 10;
	const size_t DNOTYPE = 11;
	const size_t DWRONGTYPES = 12;
}
