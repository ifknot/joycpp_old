#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <functional>
#include <cstdlib>
#include <stdexcept>

#include "colour_codes.h"

using namespace std;

using stack_t = vector<string>;
using dictionary_t = map<string, function<void()>>;

stack_t s;

bool arg(size_t n) {
	if (s.size() < n) {
		cout << RED << "stack underflow";
		return false;
	}
	else {
		return true; 
	}
}

bool flag{ true };

dictionary_t d = {
	{".",		[&]() { if (arg(1)) { cout << BOLDWHITE << s.back(); } }},
	{".s",		[&]() { for (auto& i : s) { cout << i << " "; } }},
	//quotations
	{"[",		[&]() { flag = false; s.push_back("["); }},
	{"]",		[&]() { flag = true; }},
	//combinators
	{"i",		[&]() { //read from [ -- ] constructing a string then exec the string}},
	//stack operations
	{"dup",		[&]() { if (arg(1)) { s.push_back(s.back()); } }},
	{"pop",		[&]() { if (arg(1)) { s.pop_back(); } }},
	{"swap",	[&]() { if (arg(2)) { auto x = s[s.size() - 1]; s[s.size() - 1] = s[s.size() - 2]; s[s.size() - 2] = x; } }},
	//math
	{"+",		[&]() { if (arg(2)) { double x, y; //if(num(2)) also if(list(2))
						x = stod(s.back()); 
						s.pop_back(); 
						y = stod(s.back()); 
						s.pop_back();
						s.push_back(to_string(x + y)); } }},
	{"*",		[&]() { if (arg(2)) { double x, y; 
						x = stod(s.back()); 
						s.pop_back(); 
						y = stod(s.back()); 
						s.pop_back();
						s.push_back(to_string(x * y)); } }},
	//special
	{"bye",		[&]() { std::exit(0); }}

};

int main() {

	std::cout << WHITE << "meh\n";

	while (true) {

		std::string line, t;

		while (!line.length()) {
			getline(std::cin, line);
		}

		std::stringstream ss(line);

		while (ss >> t) {
			auto it = d.find(t);
			if (it != d.end()) {
				if (flag) {
					(it->second)();
				}
				else if (t == "]") {
					flag = true;
					s.push_back(t);
				}
				else {
					s.push_back(it->first);
				}
			} 
			else {
				try {
					double x = std::stod(t);
					s.push_back(std::to_string(x));
				}
				catch (std::invalid_argument) {
					std::cout << RED << "no conversion";
				}
				catch (std::out_of_range) {
					std::cout << RED << "\aconverted value would fall out of the range of the result type";
				}
			}
		}
		std::cout << BOLDBLACK << "\tok\n" << RESET;

	}

}