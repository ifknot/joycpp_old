#include "lexer.h"
#include <cassert>

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

	void lexer::debug(size_t error_number) {
		std::cout << RED << "error #" << error_number << " : " <<  debug_messages[error_number] << "\n";
	}

	void lexer::parse(std::string&& line) {

		std::stringstream line_stream(line);
		std::string token;

		while (line_stream >> token) {

			if (stropping) {
				if (token == "[") {
					quote(stack);
				}
				else if (token == "]") {
					unquote(stack);
				}
				else {
					stack.back() += token + " ";
				}
				//std::cout << stack.back() << "\n";
			}
			else {
				if (can_parse(token, sys_atoms)) {}
				else {
					try {
						double x = std::stod(token);
						stack.push_back(token);
					}
					catch (std::invalid_argument) {
						debug(DNOCONVERSION);
					}
					catch (std::out_of_range) {
						debug(DOUTRANGE);
					}
				}

			}

		}
		
	}

	bool lexer::can_parse(token_t token, cpp_dictionary_t tokens) {
		auto it = sys_atoms.find(token);
		if (it != sys_atoms.end()) {
			(it->second)();
			return true;
		}
		else {
			return false;
		}
	}

	bool lexer::can_parse(token_t token, joy_dictionary_t tokens) {
		return false;
	}

	void lexer::quote(stack_t& stack) {
		if (!stropping) {
			stropping++;
			stack.push_back("[ ");
		}
		else {
			stropping++;
			stack.back() += "[ ";
		}
		//std::cout << stropping << "\n";
	}

	void lexer::unquote(stack_t& stack) {
		if (stropping) {
			stack.back() += "] ";
			stropping--;
			//std::cout << stropping << "\n";
		}
		else {
			debug(DNOTSTROPPING);
		}
	}

	bool lexer::is_quoted(line_t& line) {
		return ((line[0] == '[') && (line[line.size() - 2] == ']')) ? true : false;
	}

	bool lexer::quotes(size_t n, stack_t& stack) {
		if (!args(n, stack)) {
			debug(DNOARGS);
			return false;
		}

		auto i = stack.size() - 1;

		for (size_t j{ 0 }; j < n; ++j) {
			if (!is_quoted(stack[i--])) {
				debug(DNOQUOTE);
				return false;
			}
		}

		return true;
	}

	std::string lexer::unstrop(stack_t& stack) {
		auto s = stack.back();
		stack.pop_back();
		return s.substr(1, s.size() - 3);
	}

	bool lexer::is_number(std::string& token) {
		return std::regex_match(token, std::regex("[+-]?(?=.)(?:0|[1-9]\\d*)?(?:\.\\d*)?(?:\\d[eE][+-]?\\d+)?"));
	}

	bool lexer::args(size_t n, stack_t& stack) {
		if (stack.size() < n) {
			debug(DUNDERFLOW);
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

	//-----------------------------------------

	void lexer::dump(stack_t& stack) {
		std::cout << GREEN;
		for (auto rit = stack.rbegin(); rit != stack.rend(); ++rit) {
			std::cout << *rit << std::endl;
		}
		
	}

	void lexer::concat(stack_t& stack) {
		auto s = stack.back().substr(1, stack.back().size());
		stack.pop_back();
		stack.back() = stack.back().substr(0, stack.back().size() - 3);
		stack.back() += s;
	}



	//-----------------------------------------

}
