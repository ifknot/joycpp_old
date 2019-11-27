#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include <functional>
#include <cstdlib>
#include <stdexcept>

using stack_t = std::stack<double>;
using dictionary_t = std::map<std::string, std::function<void()>>;

stack_t s;

bool check(size_t n) {
	if (s.size() < n) {
		std::cout << "stack underflow";
		return false;
	}
	else {
		return true;
	}
}

dictionary_t d = {
	{".",		[&]() { if (check(1)) { std::cout << "\033[31m" << s.top(); s.pop(); } }},
	{".s",		[&]() {}},
	{"+",		[&]() { if (check(2)) { auto x = s.top(); s.pop(); s.top() += x; } }},
	{"bye",		[&]() { std::exit(0); }}

};

int main() {

	std::cout << "meh\n";

	while (true) {

		std::string line, t;

		while (!line.length()) {
			getline(std::cin, line);
		}

		std::stringstream ss(line);

		while (ss >> t) {
			auto it = d.find(t);
			if (it != d.end()) {
				(it->second)();
			} 
			else {
				try {
					double x = std::stod(t);
					s.push(x);
				}
				catch (std::invalid_argument) {
					std::cout << "no conversion could be performed\n";
				}
				catch (std::out_of_range) {
					std::cout << "converted value would fall out of the range of the result type\n";
				}
			}
		}

	}

}