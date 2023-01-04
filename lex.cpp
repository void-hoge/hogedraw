#include "lex.hpp"

bool is_space(const char c) {
	return (c == ' ' ||
			c == '\t'||
			c == '\n'||
			c == '\v'||
			c == '\f'||
			c == '\r'
		);
}

std::string operator*(std::string str, std::size_t n) {
	std::string tmp = "";
	for (std::size_t i = 0; i < n; i++) {
		tmp.append(str);
	}
	return tmp;
}


lex_t::lex_t(const std::string& str) {
	this->code_ = str;
	this->match_ = std::smatch();
}

void lex_t::skip_space() {
	std::size_t p = 0;
	while (p < this->code().size() && is_space(this->code_.at(p))) {
		p++;
	}
	this->skip(p);

}

void lex_t::skip(std::size_t len) {
	this->code_ = this->code_.substr(len);
}

bool lex_t::advance(std::regex pattern) {
	if (this->check(pattern)) {
		this->code_ = this->match_.suffix();
		return true;
	}else {
		return false;
	}
}

void lex_t::advance() {
	this->code_ = this->code_.substr(this->match().length());
}

bool lex_t::check(std::regex pattern) {
	this->skip_space();
	std::smatch m;
	if (std::regex_search(this->code_, m, pattern)) {
		this->match_ = m;
		return true;
	}else {
		this->match_ = m;
		return false;
	}
}

bool parsebool(lex_t& lex) {
	if (lex.check(std::regex("^(true|false|True|False|TRUE|FALSE)"))) {
		if (lex.match().str() == "true" ||
			lex.match().str() == "True" ||
			lex.match().str() == "TRUE") {
			lex.advance();
			return true;
		}else {
			lex.advance();
			return false;
		}
	}else {
		auto current = lex.code().substr(0, 50);
		auto error = std::string("Failed to parse a boolean value. \n\""+current+"\"");
		throw std::invalid_argument(error);
	}
}

int parseint(lex_t& lex) {
	if (lex.check(std::regex("^(\\d+)"))) {
		auto tmp = std::stoi(lex.match().str(1));
		lex.advance();
		return tmp;
	}else {
		auto current = lex.code().substr(0, 50);
		auto error = std::string("Failed to parse an integer value. \n\""+current+"\"");
		throw std::invalid_argument(error);
	}
}

std::string parsestring(lex_t& lex) {
	if (lex.check(std::regex("^\"(.+)\""))) {
		auto tmp = lex.match().str(1);
		lex.advance();
		return tmp;
	}else {
		auto current = lex.code().substr(0, 50);
		auto error = std::string("Failed to parse a string. \n\""+current+"\"");
		throw std::invalid_argument(error);		
	}
}
