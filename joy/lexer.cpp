#include "lexer.h"
#include <cassert>

namespace joy {

	void lexer::run() {
		while (true) {

			std::string line;

			while (!line.length()) {
				getline(std::cin, line);
			}
			
			parse(std::move(line));

			std::cout << BOLDBLACK << "\tok\n";
			switch (state) {
			case state_t::parsing:
				std::cout << RESET;
				break;
			case state_t::stropping:
				std::cout << GREEN;
				break;
			case state_t::pending:
			case state_t::naming:
				std::cout << YELLOW;
				break;
			case state_t::defining:
				std::cout << BOLDYELLOW;
				break;
			}

		}
	
	}

	void lexer::debug(size_t error_number) {
		std::cout << RED << "error #" << error_number << " : " <<  debug_messages[error_number] << "\n";
	}

	void lexer::list_sys(cpp_dictionary_t& dictionary) {
		std::cout << YELLOW;
		for (auto item : dictionary) {
			std::cout << item.first << " ";
		}
		std::cout << std::endl;
	}

	void lexer::list_joy(joy_dictionary_t& dictionary) {
		std::cout << YELLOW;
		for (auto item : dictionary) {
			std::cout << item.first << " == " << item.second << "\n";
		}
	}

	void lexer::parse(std::string&& line) {

		std::stringstream line_stream(line);
		token_t token; 

		while (line_stream >> token) {

			switch (state) {
			case state_t::naming:
				atom_name = token;
				if (!user_atoms.count(atom_name)) {
					user_atoms[atom_name] = "";
					state = state_t::pending;
				}
				else {
					state = state_t::parsing;
					debug(DATOMEXISTS);
				}
				break;
			case state_t::pending:
				if (token == "==") {
					state = state_t::defining;
				}
				else {
					state = state_t::parsing;
					debug(DNOTDEFINING);
				}
				break;
			case state_t::defining:
				if (token == ".") {
					state = state_t::parsing;
				}
				else if (token == ";") {
					state = state_t::naming;
				}
				else {
					user_atoms[atom_name] = user_atoms[atom_name] + token + " ";
				}
				break;
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
				if ((can_parse(token, sys_atoms)) ||
					(can_parse(token, joy_atoms)) ||				
					(can_parse(token, user_atoms)) ||
					(char_parse(token))) {}
				else {
					num_parse(token);
				}
				break;
			default:
				debug(DNOSTATE);
			}

		}
		
	}

	bool lexer::can_parse(token_t& token, cpp_dictionary_t& tokens) {
		auto it = sys_atoms.find(token);
		if (it != sys_atoms.end()) {
			(it->second)();
			return true;
		}
		else {
			return false;
		}
	}

	bool lexer::can_parse(token_t& token, joy_dictionary_t& dictionary) {
		auto it = dictionary.find(token);
		if (it != dictionary.end()) {
			auto s = it->second;
			parse(std::move(s));
			return true;
		}
		else {
			return false;
		}
	}

	bool lexer::char_parse(token_t& token) {
		if (is_tag_char(token)) {
			stack.push_back(token);
			return true;
		}
		else {
			return false;
		}
	}

	void lexer::num_parse(token_t& token) {
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

	void lexer::num_parse(token_t&& token) {
		auto s(token); num_parse(s);
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

	void lexer::name(stack_t& stack) {
		state = state_t::naming;
	}

	void lexer::define(stack_t& stack) {
		state = state_t::defining;
	}

	void lexer::undefine(stack_t& stack) {
		state = state_t::parsing;
	}

	bool lexer::is_tag_char(token_t& token) {
		return ((token.size() == 2) && (token[0] == '\''));
	}

	bool lexer::is_quoted(line_t& line) {
		return ((line[0] == '[') && (line[line.size() - 2] == ']')) ? true : false;
	}

	int lexer::as_bool(stack_t& stack) {
		auto n = stoi(stack.back());
		stack.pop_back();
		return (n) ? true : false;
	}

	double lexer::as_double(stack_t& stack) {
		auto n = stod(stack.back());
		stack.pop_back();
		return n;
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
