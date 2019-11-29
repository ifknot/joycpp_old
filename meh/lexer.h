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

#include "colour_codes.h"

namespace meh {

	class lexer {

		using stack_t = std::vector<std::string>;
		using dictionary_t = std::map<std::string, std::function<void()>>;

	public:

		void run();

		void parse(std::string&& line);

		std::string delist(stack_t& stack, size_t i);

	private:

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
		static bool is_number(std::string token);

		static size_t list_size(stack_t stack);

		static std::string to_string(stack_t stack);

		//check stack has at least n arguements 
		static bool arg(size_t n, stack_t stack);

		//check stack has at least n numbers
		static bool num(size_t n, stack_t stack);

		bool flag{ false };

		stack_t stack;

		dictionary_t dictionary = {
			{".",		[&]() { if (arg(1, stack)) { std::cout << BOLDWHITE << stack.back(); } }},
			{".s",		[&]() { for (auto& i : stack) { std::cout << i << " "; } }},
			//quotations
			{"[",		[&]() { flag = true;  stack.push_back("["); }},
			{"]",		[&]() { stack.push_back("]"); flag = false; }},
			//combinators
			{"i",		[&]() { parse(delist(stack, list_size(stack))); }},
			//stack operations
			{"dup",		[&]() { if (arg(1, stack)) { stack.push_back(stack.back()); } }},
			{"pop",		[&]() { if (arg(1, stack)) { stack.pop_back(); } }},
			{"swap",	[&]() { if (arg(2, stack)) { auto x = stack[stack.size() - 1]; stack[stack.size() - 1] = stack[stack.size() - 2]; stack[stack.size() - 2] = x; } }},
			//math
			{"+",		[&]() { if (num(2, stack)) { double x, y;
								std::stringstream ss;
								x = stod(stack.back()); 
								stack.pop_back(); 
								y = stod(stack.back()); 
								stack.pop_back();
								ss << (x + y);
								stack.push_back(ss.str()); } }},
			{"*",		[&]() { if (num(2, stack)) { double x, y;
								x = stod(stack.back()); 
								stack.pop_back(); 
								y = stod(stack.back()); 
								stack.pop_back();
								stack.push_back(std::to_string(x * y)); } }},
			//special
			{"bye",		[&]() { std::exit(0); }}
		};

	};

}
