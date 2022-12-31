#include "hogedraw.hpp"
#include "lex.hpp"
#include <iostream>
#include <string>


int main() {
	std::string str;
	std::getline(std::cin, str);
	try {
		lex_t lex(str);
		hogedraw* hoge = create_hogedraw(lex);
		hoge->mainloop();
		delete hoge;
	} catch(std::runtime_error e) {
		auto hoge = hogedraw();
		hoge.mainloop();
	}
	return 0;
}
