#include "hogedraw.hpp"
#include "lex.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

int main(const int argc, const char* argv[]) {
	auto ifs = std::ifstream(std::string(std::getenv("HOME"))+"/.hogedrawrc");
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

	nlohmann::json json;
	std::vector<std::string> images;
	auto re_filesuffix = std::regex("\\.([^.]+)$");
	for (int i = 1; i < argc; i++) {
		std::smatch match;
		std::string arg = argv[i];
		if (!std::regex_search(arg, match, re_filesuffix)) {
			std::stringstream err;
			err << "Filename extension is required: \""
				<< argv[i] << "\".";
			throw std::invalid_argument(err.str());
		}
		if (match.str(1) == "json" || match.str(1) == "txt") {
			ifs = std::ifstream(argv[i]);
			if (ifs.is_open()) {
				json = nlohmann::json::parse(ifs);
			}else {
				std::stringstream err;
				err << "Failed to open \""
					<< argv[i] << "\".";
				throw std::invalid_argument(err.str());
			}
		}else if (match.str(1) == "png") {
			images.push_back(argv[i]);
		}else {
			std::stringstream err;
			err << "Filename extension \""
				<< match.str(1)
				<< "\" is not supported.";
			throw std::invalid_argument(err.str());
		}
	}
	if (json.is_null()) {
		auto hoge = hogedraw(*option, images);
		hoge.mainloop();
	}else {
		auto hoge = hogedraw(json, *option, images);
		hoge.mainloop();
	}
	delete option;
	return 0;
}
