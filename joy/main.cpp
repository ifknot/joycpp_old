#include <iostream>
#include <string>

#include "lexer.h"


int main() {

	std::cout << "*** Joy  ***\n"
		<< "Joy is a functional stack programming language invented by Manfred von Thun (2001)\n"
		<< "C++ interpreter version 0.1 - Jeremy Thornton (2019)\n Colours define:\n"
		<< RED << "\terrors\n"
		<< GREEN << "\tstack printout\n"
		<< BOLDGREEN << "\tinside a list definition\n" << RESET
		<< YELLOW << "\tfunction printout\n"
		<< BOLDYELLOW << "\tinside a function definition\n" << RESET
		<< BOLDBLACK << "\tmessages\n"
		<< BOLDWHITE << "\tinteractive\n\n"
		;

	joy::lexer joy_lexer;

	joy_lexer.run();


}