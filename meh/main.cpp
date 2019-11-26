#include <iostream>
#include <string>
#include <sstream>

int main() {

	std::cout << "meh\n";

	while (true) {

		std::string s, t;

		while (!s.length()) {
			getline(std::cin, s);
		}

		if (s == "q") {
			break;
		}

		std::stringstream ss(s);

		while (ss >> t) {
			std::cout << t << "\n";
		}

	}

}