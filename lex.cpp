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
	while (this->code_.size() < p && is_space(this->code_.at(p))) {
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
