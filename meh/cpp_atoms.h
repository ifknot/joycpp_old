#pragma once

cpp_dictionary_t sys_atoms = {
{".",		[&]() { if (args(1, stack)) { std::cout << GREEN << stack.back(); } }},
{".s",		[&]() { std::cout << GREEN;
					for (auto rit = stack.rbegin(); rit != stack.rend(); ++rit) {
						std::cout << *rit << std::endl;
					}	}},
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
//stack operations
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
				{"quit",		[&]() { std::exit(0); }}	//Exit from Meh (Joy).
};