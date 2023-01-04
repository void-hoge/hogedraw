#include "hogedraw.hpp"
#include "lex.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(const int argc, const char* argv[]) {
	auto ifs = std::ifstream(std::string(std::getenv("HOME"))+"/.hogedrawrc");
//	auto ifs = std::ifstream(".hogedrawrc");
	option_t* option;
	if (ifs.is_open()) {
		std::string tmp;
		std::vector<std::string> lines;
		while (std::getline(ifs, tmp)) {
			lines.push_back(tmp);
		}
		ifs.close();
		option = new option_t(lines);
	}else {
		option = new option_t();
	}

	hogedraw* hoge;
	if (argc == 2) {
		ifs = std::ifstream(argv[1]);
		if (ifs.is_open()) {
			std::string tmp;
			std::string savecode;
			while (std::getline(ifs, tmp)) {
				savecode += tmp;
			}
			ifs.close();
			lex_t lex(savecode);
			hoge = create_hogedraw(lex, *option);
		}else {
			hoge = new hogedraw(*option);
		}
	}else {
		hoge = new hogedraw(*option);
	}
	hoge->mainloop();
	delete hoge;
	return 0;
}
