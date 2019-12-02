#include "lexer.h"

namespace meh {

	void lexer::run() {
		while (true) {

			std::string line;

			while (!line.length()) {
				getline(std::cin, line);
			}
			
			parse(std::move(line));

			std::cout << BOLDBLACK << "\tok\n" << ((stropping) ? GREEN : RESET);

		}
	
	}

	void lexer::parse(std::string&& line) {

		std::stringstream line_stream(line);
		std::string token;

		while (line_stream >> token) {

			if (stropping) {
				if (token == "[") {
					//TODO
				}
				stack.back() += token + " ";
			}
			else {

				auto it = dictionary.find(token);

				if (it != dictionary.end()) {
					(it->second)();
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
	}

	void lexer::quote(stack_t& stack) {
		stropping++;
		stack.push_back("[ ");
	}

	void lexer::unquote(stack_t& stack) {
		stack.back() += "]";
		stropping--;
	}

	void lexer::dump(stack_t& stack) {
		std::cout << GREEN;
		for (const auto& token : stack) {
			std::cout << token << " ";
		}
	}

	std::string lexer::unstrop(stack_t& stack) {
		auto s = stack.back();
		stack.pop_back();
		return s.substr(1, s.size() - 2);
	}

	lexer::strops_t lexer::find_strops(stack_t& stack, std::string begin, std::string end) {

		strops_t strops{ 0,0 };

		if (stack.back() != end) {
			std::cout << RED << "malformed stack top - missing " << end;
			return strops;
		}

		if (!args(3, stack)) {
			return strops;
		}

		size_t i{ stack.size() - 1 };
		size_t n{ 0 };

		while (i >= 0) {

			if (stack[i] == end) {
				if (n == 0) {
					//std::cout << b << " at " << i << "\n";
					strops.second = i;
				}
				++n;
			}

			if (stack[i] == begin) {
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

	bool lexer::is_number(std::string& token) {
		return std::regex_match(token, std::regex("[+-]?(?=.)(?:0|[1-9]\\d*)?(?:\.\\d*)?(?:\\d[eE][+-]?\\d+)?"));
	}

	bool lexer::args(size_t n, stack_t& stack) {
		if (stack.size() < n) {
			std::cout << RED << "stack underflow\n";
			dump(stack);
			return false;
		}
		else {
			return true;
		}
	}

	bool lexer::nums(size_t n, stack_t& stack) {
		if (!args(n, stack)) {
			return false;
		}

		auto i = stack.size() - 1;
		
		for (size_t j{ 0 }; j < n; ++j) {
			if (!is_number(stack[i--])) {
				return false;
			}
		}

		return true;
	}

	bool lexer::is_quoted(line_t& line) {
		return ((line[0] == '[') && (line[line.size() - 1] == ']')) ? true : false;
	}

	bool lexer::quotes(size_t n, stack_t& stack) {
		if (!args(n, stack)) {
			return false;
		}

		auto i = stack.size() - 1;

		for (size_t j{ 0 }; j < n; ++j) {
			if (!is_quoted(stack[i--])) {
				return false;
			}
		}

		return true;
	}

}
