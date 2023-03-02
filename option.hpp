#ifndef OPTION_HPP
#define OPTION_HPP

#include <string>
#include <array>

#include "vec.hpp"
#include "lex.hpp"

class option_t {
private:
	void init();
public:
	std::array<color_t, 6> colors;
	std::string fontpath;
	int fontsize;
	int linethickness;
	int eraserthickness;
	bool trianglefill;
	int trianglesize;
	int trianglethickness;
	bool squarefill;
	int squaresize;
	int squarethickness;
	bool circlefill;
	int circlesize;
	int circlethickness;

	option_t();
	option_t(std::vector<std::string>& lines);
	void dump(std::ostream& ost);
};

#endif // include guard of OPTION_HPP
