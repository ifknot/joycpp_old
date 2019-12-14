#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <functional>
#include <cstdlib>
#include <stdexcept>
#include <regex>
#include <math.h>

#include "colour_codes.h"
#include "error_messages.h"

namespace joy {

	class lexer {

		using token_t = std::string;
		using line_t = std::string;
		using stack_t = std::vector<line_t>;
		using cpp_dictionary_t = std::map<token_t, std::function<void()>>;
		using joy_dictionary_t = std::map<token_t, line_t>;
		using strops_t = std::pair<size_t, size_t>;

		enum class state_t { parsing, stropping, naming, pending, defining };

		state_t state{ state_t::parsing };
		size_t strops{ 0 };
		stack_t stack;
		token_t atom_name;

	public:

		void run();

	private:

		static void debug(size_t error_number);

		static void list_sys(cpp_dictionary_t& dictionary);

		static void list_joy(joy_dictionary_t& dictionary);

		void parse(line_t&& line);

		bool can_parse(token_t token, cpp_dictionary_t tokens);

		bool can_parse(token_t token, joy_dictionary_t tokens);

		void pod_parse(token_t token);

		void quote(stack_t& stack);

		void unquote(stack_t& stack);

		void name(stack_t& stack);

		void define(stack_t& stack);

		void undefine(stack_t& stack);

		/**
		 * allowed:
		 * +3
		 * 3.2e23
		 * -4.70e+9
		 * -.2E-4
		 * -7.6603
		 *
		 * not allowed:
		 * +0003   (leading zeros)
		 * 37.e88  (dot before the e)
		 */
		static bool is_number(token_t& token);

		//check if string is quoted program or list
		static bool is_quoted(line_t& line);

		//check stack has at least n quoted program(s) or list(s)
		static bool quotes(size_t n, stack_t& stack);

		// remove [ ] & space
		static std::string unstrop(stack_t& stack);

		//check stack has at least n arguement(s) 
		static bool args(size_t n, stack_t& stack);

		//check stack has at least n number(s)
		static bool nums(size_t n, stack_t& stack);

		//------------------------------------------

		cpp_dictionary_t sys_atoms = {
//special
{".",		[&]() { if (args(1, stack)) { std::cout << GREEN << stack.back(); } }},
//defines
{"SYSDEF", [&]() { list_sys(sys_atoms); }}, 
{"JOYDEF", [&]() { list_joy(joy_atoms); }},
{"USERDEF", [&]() { list_joy(user_atoms); }},
{"DEFINE",	[&]() { name(stack); }},
{"==",		[&]() { define(stack); }},
{";",		[&]() { name(stack); }},
//lists
{"[",		[&]() { quote(stack); }},
{"]",		[&]() { unquote(stack); }},
{"cons",	[&]() { if (args(2, stack) && quotes(1, stack)) {  
					auto s = stack.back().substr(1, stack.back().size());
					stack.pop_back();
					stack.back() = "[ " + stack.back() + s;} }},
{"append",  [&]() { if (args(2, stack) && quotes(1, stack)) {
					auto s = stack.back().substr(0, stack.back().size() - 2);
					stack.pop_back();
					stack.back() = s + stack.back() + " ]";
					} }},
{"concat",	[&]() { if (quotes(2, stack)) {
					auto s = stack.back().substr(1, stack.back().size());
					stack.pop_back();
					stack.back() = stack.back().substr(0, stack.back().size() - 3);
					stack.back() += s;
					} }}, 
//combinators
{"i",		[&]() { if (quotes(1, stack)) { parse(unstrop(stack)); } }},
{"map",		[&]() { if (quotes(2, stack)) {
					line_t prog(unstrop(stack));
					std::stringstream args(unstrop(stack)); 
					token_t arg, atom;
					line_t result = "[ ";
					stack.push_back("");
					while (args >> arg) {
						stack.back() = stack.back() + arg + " ";
						std::stringstream atoms(prog);
						while (atoms >> atom) {
							stack.back() = stack.back() + atom + " ";
						}
						auto s = stack.back();
						stack.pop_back();
						parse(std::move(s));
						result = result + stack.back() + " ";
						stack.pop_back();
						stack.push_back("");
					}
					result += "] ";
					stack.push_back(result);
					} }},
//stack operations
{".s",		[&]() { std::cout << GREEN;
					for (auto rit = stack.rbegin(); rit != stack.rend(); ++rit) {
						std::cout << *rit << std::endl;
					}	}},
{"dup",		[&]() { if (args(1, stack)) { stack.push_back(stack.back()); } }},
{"pop",		[&]() { if (args(1, stack)) { stack.pop_back(); } }},
{"swap",	[&]() { if (args(2, stack)) {
					auto x = stack[stack.size() - 1];
					stack[stack.size() - 1] = stack[stack.size() - 2];
					stack[stack.size() - 2] = x;
					}
					}},
//math
{"true",	[&]() { pod_parse("1"); }},
{"false",	[&]() { pod_parse("0"); }},
{"+",		[&]() { if (nums(2, stack)) {
					auto y = stod(stack.back());
					stack.pop_back();
					auto x = stod(stack.back());
					stack.pop_back();
					stack.push_back(std::to_string(x + y));
			}}},
{"-",		[&]() { if (nums(2, stack)) {
					auto y = stod(stack.back());
					stack.pop_back();
					auto x = stod(stack.back());
					stack.pop_back();
					stack.push_back(std::to_string(x - y));
			}}},
{"*",		[&]() { if (nums(2, stack)) {
					auto y = stod(stack.back());
					stack.pop_back();
					auto x = stod(stack.back());
					stack.pop_back();
					stack.push_back(std::to_string(x * y));
			}}},
{"/",		[&]() { if (nums(2, stack)) {
					auto y = stod(stack.back());
					stack.pop_back();
					auto x = stod(stack.back());
					stack.pop_back();
					stack.push_back(std::to_string(x / y));
			}}},
{"rem",		[&]() { if (nums(2, stack)) {
					auto y = stod(stack.back());
					stack.pop_back();
					auto x = stod(stack.back());
					stack.pop_back();
					stack.push_back(std::to_string(fmod(x, y)));
			}}},
{"abs",		[&]() { if (nums(1, stack)) {
					auto x = stod(stack.back());
					stack.pop_back();
					stack.push_back(std::to_string(abs(x)));
			}}},
{"signum",	[&]() { if (nums(1, stack)) {
					auto x = stod(stack.back());
					stack.pop_back();
					stack.push_back(std::to_string((x > 0) - (x < 0)));
			}}},
				//io
				{},
				{},
				//special
				{"quit",		[&]() { std::exit(0); }}	//Exit from Joy.
};

joy_dictionary_t joy_atoms {
{"swons", "swap cons" }
};

joy_dictionary_t user_atoms {};

		}; 

		}

	

