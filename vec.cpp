#include "vec.hpp"
#include "lex.hpp"

template<>
vec2<int>::vec2(lex_t& lex) {
	if (lex.check(std::regex("\\((\\d+),(\\d+)\\)"))) {
		this->x() = std::stoi(lex.match().str(1));
		this->y() = std::stoi(lex.match().str(2));
		lex.advance();
	}else {
		auto current = lex.code().substr(0, 50);
		auto error = std::string("Failed to parse a vec2<int> value. \n\""+current+"\"");
		throw std::invalid_argument(error);
	}
}

template<>
vec3<int>::vec3(lex_t& lex) {
	if (lex.check(std::regex("\\((\\d+),(\\d+),(\\d+)\\)"))) {
		this->x() = std::stoi(lex.match().str(1));
		this->y() = std::stoi(lex.match().str(2));
		this->z() = std::stoi(lex.match().str(3));
		lex.advance();
	}else {
		auto current = lex.code().substr(0, 50);
		auto error = std::string("Failed to parse a vec3<int> value. \n\""+current+"\"");
		throw std::invalid_argument(error);
	}
}
