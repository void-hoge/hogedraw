#ifndef LEX_HPP
#define LEX_HPP

#include <string>
#include <regex>
#include <istream>

bool is_space(const char c);

std::string operator*(std::string str, std::size_t n);
class lex_t {
private:
	std::string code_;
	std::smatch match_;
public:
	lex_t(const std::string& str);
	void skip_space();
	void skip(std::size_t len);
	bool check(std::regex pattern);
	bool advance(std::regex pattern);
	void advance();
	const std::string& code()const {
		return code_;
	}
	const std::smatch& match()const {
		return match_;
	}	
};

#endif // include guard of LEX_HPP
