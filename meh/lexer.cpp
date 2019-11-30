#include "lexer.h"

namespace meh {

	void lexer::run() {
		while (true) {

			std::string line;

			while (!line.length()) {
				getline(std::cin, line);
			}
			
			parse(std::move(line));

			std::cout << BOLDBLACK << "\tok\n" << ((flag) ? GREEN : RESET);

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

	std::string lexer::unstrop(stack_t& stack, strops_t strops) {

		std::string s;
		
		if (strops.first < strops.second) {
		
			for(auto i = strops.first + 1; i < strops.second; ++i) {
				s += stack[i] + " ";
			}
			
			while (stack.size() > strops.first) {
				stack.pop_back();
			}

		}

		std::cout << s << "\n";
		return s;
	}

	lexer::strops_t lexer::find_strops(stack_t& stack, std::string a, std::string b) {

		strops_t strops{ 0,0 };

		if (stack.back() != b) {
			std::cout << RED << "malformed stack top - missing " << b;
			return strops;
		}

		if (!arg(3, stack)) {
			return strops;
		}

		size_t i{ stack.size() - 1 };
		size_t n{ 0 };

		while (i >= 0) {

			if (stack[i] == b) {
				if (n == 0) {
					//std::cout << b << " at " << i << "\n";
					strops.second = i;
				}
				++n;
			}

			if (stack[i] == a) {
				--n;
				if (n == 0) {
					//std::cout << a << " at " << i << "\n";
					strops.first = i;
					return strops;
				}
			}

			--i;
		}

		return strops;
	}

	bool lexer::is_number(std::string token) {
		return std::regex_match(token, std::regex("[+-]?(?=.)(?:0|[1-9]\\d*)?(?:\.\\d*)?(?:\\d[eE][+-]?\\d+)?"));
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
