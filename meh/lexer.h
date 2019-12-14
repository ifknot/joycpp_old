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

namespace meh {

	class lexer {

		using token_t = std::string;
		using line_t = std::string;
		using stack_t = std::vector<line_t>;
		using cpp_dictionary_t = std::map<token_t, std::function<void()>>;
		using joy_dictionary_t = std::map<token_t, line_t>;
		using strops_t = std::pair<size_t, size_t>;

	public:

		void run();

	private:

		static void debug(size_t error_number);

		void parse(line_t&& line);

		bool can_parse(token_t token, cpp_dictionary_t tokens);

		bool can_parse(token_t token, joy_dictionary_t tokens);

		void pod_parse(token_t token);

		void quote(stack_t& stack);

		void unquote(stack_t& stack);

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

		// remove [, ] & space
		static std::string unstrop(stack_t& stack);

		//check stack has at least n arguement(s) 
		static bool args(size_t n, stack_t& stack);

		//check stack has at least n number(s)
		static bool nums(size_t n, stack_t& stack);

		size_t stropping{ 0 };

		stack_t stack;

		#include "cpp_atoms.h"

		#include "joy_atoms.h"

		#include "user_atoms.h"

	};

}

