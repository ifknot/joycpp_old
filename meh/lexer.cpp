#include "lexer.h"

namespace meh {

	void lexer::run() {
		while (true) {

			std::string line;

			while (!line.length()) {
				getline(std::cin, line);
			}
			
			parse(std::move(line));

			std::cout << BOLDBLACK << "\tok\n" << RESET;

		}
	
	}

	void lexer::parse(std::string&& line) {

		std::stringstream line_stream(line);
		std::string token;

		while (line_stream >> token) {
			auto it = dictionary.find(token);
			if (it != dictionary.end()) {
				if (flag && token != "]") {
					stack.push_back(token);
				}
				else {
					(it->second)();
				}
			}
			else {
				try {
					double x = std::stod(token);
					stack.push_back(token);
				}
				catch (std::invalid_argument) {
					std::cout << RED << "no conversion";
				}
				catch (std::out_of_range) {
					std::cout << RED << "\aconverted value would fall out of the range of the result type";
				}
			}
		}
	}

	std::string lexer::delist(stack_t& stack, size_t i) {
		std::stringstream line;
		if (i) {
			stack.pop_back();
			while (i--) {
				line << stack.back() << " "; //wrong order
				stack.pop_back();
			}
			stack.pop_back();
		}
		return line.str();
	}
	
	bool lexer::is_number(std::string token) {
		return std::regex_match(token, std::regex("[+-]?(?=.)(?:0|[1-9]\\d*)?(?:\.\\d*)?(?:\\d[eE][+-]?\\d+)?"));
		//return std::regex_match(token, std::regex("[+-]?(\\d+([.]\\d*)?(e[+-]?\\d+)?|[.]\\d+(e[+-]?\\d+)?)"));
		//return std::regex_match(token, std::regex("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?"));
	}

	size_t lexer::list_size(stack_t stack) {
		//counters for [ & ] to handle nested lists
		if (!arg(2, stack)) {
			return 0;
		}
		if (stack.back() != "]") {
			std::cout << RED << "no list on stack - missing ]";
			return 0;
		}
		auto i = stack.size() - 2;
		for (size_t j{ 0 }; j < stack.size() - 2; ++j) {
			if (stack[i] == "]") {
				std::cout << RED << "invalid nested list on stack";
				return 0;
			}
			if (stack[i--] == "[") {
				return j;
			}
		}
		std::cout << RED << "no list on stack - missing [";
		return false;
	}


	bool lexer::arg(size_t n, stack_t stack) {
		if (stack.size() < n) {
			std::cout << RED << "stack underflow";
			return false;
		}
		else {
			return true;
		}
	}

	bool lexer::num(size_t n, stack_t stack) {
		if (!arg(n, stack)) {
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
