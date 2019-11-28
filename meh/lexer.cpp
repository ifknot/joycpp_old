#include "lexer.h"

namespace meh {

	void lexer::run() {
		while (true) {

			std::string line;

			while (!line.length()) {
				getline(std::cin, line);
			}
			
			try {
				parse(line);
			}
			catch (std::invalid_argument) {
				std::cout << RED << "no conversion";
			}
			catch (std::out_of_range) {
				std::cout << RED << "\aconverted value would fall out of the range of the result type";
			}

			std::cout << BOLDBLACK << "\tok\n" << RESET;

		}
	
	}

	void lexer::parse(std::string& line) {

		std::stringstream line_stream(line);
		std::string token;

		while (line_stream >> token) {
			auto it = dictionary.find(token);
			if (it != dictionary.end()) {
				if (list && token != "]") {
					stack.push_back(token);
				}
				else {
					(it->second)();
				}
			}
			else {
				double x = std::stod(token);
				stack.push_back(token);
			}
		}
	}

	std::string lexer::delist(stack_t& stack) {

		return std::string();
	}
	
	bool lexer::is_number(std::string token) {
		return std::regex_match(token, std::regex("[+-]?(?=.)(?:0|[1-9]\\d*)?(?:\.\\d*)?(?:\\d[eE][+-]?\\d+)?"));
		//return std::regex_match(token, std::regex("[+-]?(\\d+([.]\\d*)?(e[+-]?\\d+)?|[.]\\d+(e[+-]?\\d+)?)"));
		//return std::regex_match(token, std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
	}

	bool lexer::arg(size_t n) {
		if (stack.size() < n) {
			std::cout << RED << "stack underflow";
			return false;
		}
		else {
			return true;
		}
	}

	bool lexer::num(size_t n) {
		if (!arg(n)) {
			return false;
		}
		auto i = stack.size() - 1;
		for (size_t j{ 0 }; j < n; ++j) {
			if (!is_number(stack[i])) {
				return false;
			}
		}
		return true;
	}

}
