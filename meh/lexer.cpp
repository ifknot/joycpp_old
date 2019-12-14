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

			std::cout << BOLDBLACK << "\tok\n" << ((strops) ? GREEN : RESET);

		}
	
	}

	void lexer::debug(size_t error_number) {
		std::cout << RED << "error #" << error_number << " : " <<  debug_messages[error_number] << "\n";
	}

	void lexer::parse(std::string&& line) {

		std::stringstream line_stream(line);
		std::string token;

		while (line_stream >> token) {

			switch (state) {
			case state_t::stropping:
				if (token == "[") {
					quote(stack);
				}
				else if (token == "]") {
					unquote(stack);
				}
				else {
					stack.back() += token + " ";
				}
				break;
			case state_t::parsing:
				if (can_parse(token, sys_atoms)) {}
				else if (can_parse(token, joy_atoms)) {}
				//else if (can_parse(token, user_atoms)) {}
				else {
					pod_parse(token);
				}
				break;
			default:
				debug(DNOCONVERSION);
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
		auto it = joy_atoms.find(token);
		if (it != joy_atoms.end()) {
			auto s = it->second;
			parse(std::move(s));
			return true;
		}
		else {
			return false;
		}
	}

	void lexer::pod_parse(token_t token) {
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

	void lexer::quote(stack_t& stack) {
		if (!strops) {
			strops++;
			stack.push_back("[ ");
			state = state_t::stropping;
		}
		else {
			strops++;
			stack.back() += "[ ";
		}
	}

	void lexer::unquote(stack_t& stack) {
		if (strops) {
			stack.back() += "] ";
			strops--;
			if (strops == 0) state =  state_t::parsing;
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

}
