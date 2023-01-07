#include "option.hpp"

#include <iostream>

void option_t::init() {
	this->colors.at(1) = color_t(255,255,255);
	this->fontpath = "/usr/share/fonts/truetype/freefont/FreeMono.ttf";
	this->fontsize = 50;
	this->linethickness = 2;
	this->trianglefill = false;
	this->trianglesize = 50;
	this->trianglethickness = 2;
	this->squarefill = false;
	this->squaresize = 50;
	this->trianglethickness = 2;
	this->circlefill = false;
	this->circlesize = 50;
	this->circlethickness = 2;
}

option_t::option_t() {
	this->init();
}

option_t::option_t(std::vector<std::string>& lines) {
	this->init();
	for (auto line: lines) {
		lex_t lex(line);
		try {
			if (lex.check(std::regex("^color(0|1|2|3|4|5):"))) {
				auto idx = std::stoi(lex.match().str(1));
				lex.advance();
				this->colors.at(idx) = vec3<int>(lex);
			}else if (lex.check(std::regex("^fontpath:"))) {
				lex.advance();
				this->fontpath = parsestring(lex);
			}else if (lex.check(std::regex("^fontsize:"))) {
				lex.advance();
				this->fontsize = parseint(lex);
			}else if (lex.check(std::regex("^linethickness:"))) {
				lex.advance();
				this->linethickness = parseint(lex);
			}else if (lex.check(std::regex("^circlefill:"))) {
				lex.advance();
				this->circlefill = parsebool(lex);
			}else if (lex.check(std::regex("^circlesize:"))) {
				lex.advance();
				this->circlesize = parseint(lex);
			}else if (lex.check(std::regex("^circlethickness:"))) {
				lex.advance();
				this->circlethickness = parseint(lex);
			}else if (lex.check(std::regex("^trianglefill:"))) {
				lex.advance();
				this->trianglefill = parsebool(lex);
			}else if (lex.check(std::regex("^trianglesize:"))) {
				lex.advance();
				this->trianglesize = parseint(lex);
			}else if (lex.check(std::regex("^trianglethickness:"))) {
				lex.advance();
				this->trianglethickness = parseint(lex);
			}else if (lex.check(std::regex("^squarefill:"))) {
				lex.advance();
				this->squarefill = parsebool(lex);
			}else if (lex.check(std::regex("^squaresize:"))) {
				lex.advance();
				this->squaresize = parseint(lex);
			}else if (lex.check(std::regex("^squarethickness:"))) {
				lex.advance();
				this->squarethickness = parseint(lex);
			}else {
				std::cerr << "There is no such option.\"" << line << "\"" << std::endl;
			}
		} catch(std::invalid_argument& e) {
			std::cerr << e.what() << std::endl;
			throw std::invalid_argument("Failed to parse \""+line+"\"");
		}
		
	}
}

void option_t::dump(std::ostream& ost) {
	ost << std::boolalpha
		<< "color0" << this->colors.at(0) << std::endl
		<< "color1" << this->colors.at(1) << std::endl
		<< "color2" << this->colors.at(2) << std::endl
		<< "color3" << this->colors.at(3) << std::endl
		<< "color4" << this->colors.at(4) << std::endl
		<< "color5" << this->colors.at(5) << std::endl
		<< "fontpath:" << this->fontpath << std::endl
		<< "fontsize:" << this->fontsize << std::endl
		<< "trianglefill:" << this->trianglefill << std::endl
		<< "trianglesize:" << this->trianglesize << std::endl
		<< "squarefill:" << this->squarefill << std::endl
		<< "squaresize:" << this->squaresize << std::endl
		<< "circlefill:" << this->circlefill << std::endl
		<< "circlesize:" << this->circlesize << std::endl;
}
