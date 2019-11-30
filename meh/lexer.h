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

namespace meh {

	class lexer {

		using stack_t = std::vector<std::string>;
		using dictionary_t = std::map<std::string, std::function<void()>>;
		using strops_t = std::pair<size_t, size_t>;

	public:

		void run();

	private:

		void parse(std::string&& line);

		static std::string unstrop(stack_t& stack, strops_t strops);

		static strops_t find_strops(stack_t& stack, std::string a, std::string b);

		static std::string concat(stack_t& stack) { return ""; }

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

		//check stack has at least n arguements 
		static bool arg(size_t n, stack_t stack);

		//check stack has at least n numbers
		static bool num(size_t n, stack_t stack);

		bool flag{ false };

		stack_t stack;

		dictionary_t dictionary = {
			{".",		[&]() { if (arg(1, stack)) { std::cout << GREEN << stack.back(); } }},
			{".s",		[&]() { std::cout << GREEN; 
								for (auto & i : stack) { 
									std::cout << i << " "; 
								}
						}},
			//lists
			{"[",		[&]() { flag = true; stack.push_back("["); }},
			{"]",		[&]() { stack.push_back("]"); flag = false; }},
			//{"concat",	[&]() { concat(stack); }}, //strings and lists by pop " or ] and then concat(stack, ")
			//combinators
			{"i",		[&]() { parse(unstrop(stack, find_strops(stack, "[", "]"))); }},
			//stack operations
			{"dup",		[&]() { if (arg(1, stack)) { stack.push_back(stack.back()); } }},
			{"pop",		[&]() { if (arg(1, stack)) { stack.pop_back(); } }},
			{"swap",	[&]() { if (arg(2, stack)) { auto x = stack[stack.size() - 1]; 
													 stack[stack.size() - 1] = stack[stack.size() - 2]; 
													 stack[stack.size() - 2] = x; } 
						}},
			//math
			{"+",		[&]() { if (num(2, stack)) { 
								auto y = stod(stack.back());
								stack.pop_back();
								auto x = stod(stack.back());
								stack.pop_back();
								stack.push_back(std::to_string(x + y));
						}}},
			{"-",		[&]() { if (num(2, stack)) { 
								auto y = stod(stack.back());
								stack.pop_back();
								auto x = stod(stack.back());
								stack.pop_back();
								stack.push_back(std::to_string(x - y));
						}}},
			{"*",		[&]() { if (num(2, stack)) { 
								auto y = stod(stack.back());
								stack.pop_back(); 
								auto x = stod(stack.back());
								stack.pop_back();
								stack.push_back(std::to_string(x * y)); 
						}}},
			{"/",		[&]() { if (num(2, stack)) { 
								auto y = stod(stack.back());
								stack.pop_back(); 
								auto x = stod(stack.back());
								stack.pop_back();
								stack.push_back(std::to_string(x / y)); 
						}}},
			{"rem",		[&]() { if (num(2, stack)) { 
								auto y = stod(stack.back());
								stack.pop_back(); 
								auto x = stod(stack.back());
								stack.pop_back();
								stack.push_back(std::to_string(fmod(x, y))); 
						}}},
			{"abs",		[&]() { if (num(1, stack)) { 
								auto x = stod(stack.back());
								stack.pop_back();
								stack.push_back(std::to_string(abs(x))); 
						}}},
			{"signum",	[&]() { if (num(1, stack)) { 
								auto x = stod(stack.back());
								stack.pop_back();
								stack.push_back(std::to_string((x > 0) - (x < 0)));
						}}},
			//io
			{},
			{},
			//special
			{"quit",		[&]() { std::exit(0); }}	//Exit from Meh (Joy).
		};

	};

}

