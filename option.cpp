#include "option.hpp"

#include <iostream>

std::string option_t::concate_tokens() {
	std::string tmp = "^(";
	tmp += this->tokens.at(0);
	for (std::size_t i = 1; i < this->tokens.size(); i++) {
		tmp += std::string("|") + this->tokens.at(i);
	}
	tmp += "):";
	return tmp;
}

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
	const auto pattern = this->concate_tokens();
	int linenum = 0;
	for (auto line: lines) {
		linenum++;
		lex_t lex(line);
		if (!lex.check(std::regex(pattern))) {
			auto error = std::string("syntax error at line ") + std::to_string(linenum);
			throw std::runtime_error(error);
		}
		if (lex.match().str(1) == tokens.at(0)) { // color0
			lex.advance();
			if (lex.check(std::regex("\\((\\d+),(\\d+),(\\d+)\\)"))) {
				this->colors.at(0).x() = std::stoi(lex.match().str(1));
				this->colors.at(0).y() = std::stoi(lex.match().str(2));
				this->colors.at(0).z() = std::stoi(lex.match().str(3));
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(0) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(1)) { // color1
			lex.advance();
			if (lex.check(std::regex("\\((\\d+),(\\d+),(\\d+)\\)"))) {
				this->colors.at(1).x() = std::stoi(lex.match().str(1));
				this->colors.at(1).y() = std::stoi(lex.match().str(2));
				this->colors.at(1).z() = std::stoi(lex.match().str(3));
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(1) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(2)) { // color2
			lex.advance();
			if (lex.check(std::regex("\\((\\d+),(\\d+),(\\d+)\\)"))) {
				this->colors.at(2).x() = std::stoi(lex.match().str(1));
				this->colors.at(2).y() = std::stoi(lex.match().str(2));
				this->colors.at(2).z() = std::stoi(lex.match().str(3));
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(2) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(3)) { // color3
			lex.advance();
			if (lex.check(std::regex("\\((\\d+),(\\d+),(\\d+)\\)"))) {
				this->colors.at(3).x() = std::stoi(lex.match().str(1));
				this->colors.at(3).y() = std::stoi(lex.match().str(2));
				this->colors.at(3).z() = std::stoi(lex.match().str(3));
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(3) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(4)) { // color4
			lex.advance();
			if (lex.check(std::regex("\\((\\d+),(\\d+),(\\d+)\\)"))) {
				this->colors.at(4).x() = std::stoi(lex.match().str(1));
				this->colors.at(4).y() = std::stoi(lex.match().str(2));
				this->colors.at(4).z() = std::stoi(lex.match().str(3));
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(4) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(5)) { // color5
			lex.advance();
			if (lex.check(std::regex("\\((\\d+),(\\d+),(\\d+)\\)"))) {
				this->colors.at(5).x() = std::stoi(lex.match().str(1));
				this->colors.at(5).y() = std::stoi(lex.match().str(2));
				this->colors.at(5).z() = std::stoi(lex.match().str(3));
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(5) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(6)) { // linethickness
			lex.advance();
			if (lex.check(std::regex("(\\d+)"))) {
				this->linethickness = std::stoi(lex.match().str(1));
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(6) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(7)) { // circlefill
			lex.advance();
			if (lex.check(std::regex("(true|false)"))) {
				if (lex.match().str() == "true") {
					this->circlefill = true;
				}else {
					this->circlefill = false;
				}
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(7) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(8)) { // circlesize
			lex.advance();
			if (lex.check(std::regex("(\\d+)"))) {
				this->circlesize = std::stoi(lex.match().str());
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(8) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(9)) { // circlethickness
			lex.advance();
			if (lex.check(std::regex("(\\d+)"))) {
				this->circlethickness = std::stoi(lex.match().str());
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(9) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(10)) { // trianglefill
			lex.advance();
			if (lex.check(std::regex("(true|false)"))) {
				if (lex.match().str() == "true") {
					this->trianglefill = true;
				}else {
					this->trianglefill = false;
				}
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(10) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(11)) { // trianglesize
			lex.advance();
			if (lex.check(std::regex("(\\d+)"))) {
				this->trianglesize = std::stoi(lex.match().str());
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(11) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(12)) { // trianglethickness
			lex.advance();
			if (lex.check(std::regex("(\\d+)"))) {
				this->trianglethickness = std::stoi(lex.match().str());
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(12) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(13)) { // squarefill
			lex.advance();
			if (lex.check(std::regex("(true|false)"))) {
				if (lex.match().str() == "true") {
					this->squarefill = true;
				}else {
					this->squarefill = false;
				}
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(13) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(14)) { // sqaresize
			lex.advance();
			if (lex.check(std::regex("(\\d+)"))) {
				this->squaresize = std::stoi(lex.match().str());
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(14) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(15)) { // sqarethickness
			lex.advance();
			if (lex.check(std::regex("(\\d+)"))) {
				this->squarethickness = std::stoi(lex.match().str());
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(15) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(16)) {
			lex.advance();
			if (lex.check(std::regex("\"(.+)\""))) {
				this->fontpath = lex.match().str(1);
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(16) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
		}else if (lex.match().str(1) == tokens.at(17)) {
			lex.advance();
			if (lex.check(std::regex("(\\d+)"))) {
				this->fontsize = std::stoi(lex.match().str());
				lex.advance();
			}else {
				auto error = std::string("syntax error while parsing ")
					+ this->tokens.at(17) + ", at line "
					+ std::to_string(linenum);
				throw std::runtime_error(error);
			}
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
