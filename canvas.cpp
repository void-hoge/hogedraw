#include "canvas.hpp"
#include "util.hpp"
#include <iostream>

canvas* create_canvas(lex_t& lex, FTPixmapFont* f) {
	if (lex.advance(std::regex("^canvas:"))) {
		return new canvas(lex, f);
	}else {
		throw std::runtime_error("syntax error at create_canvas");
	}
}

canvas::canvas() {}

canvas::canvas(lex_t& lex, FTPixmapFont* f) {
	if (!lex.advance(std::regex("^\\{"))) {
		throw std::runtime_error("syntax error at canvas");
	}
	while (lex.check(std::regex("^(text|line|regpoly):"))) {
		this->objects.push_back(create_object(lex, f));
		lex.advance(std::regex("^,"));
	}
	if (!lex.advance(std::regex("^\\}"))) {
		throw std::runtime_error("syntax error at canvas");
	}
}

canvas::~canvas() {
	for (auto obj: this->objects) {
		delete obj;
	}
	this->objects.clear();
}

void canvas::render(vec2<int>windowsize) const {
	for (auto obj: this->objects) {
		obj->render(windowsize);
	}
}

void canvas::push_back(object* obj) {
	this->objects.push_back(obj);
}

void canvas::undo() {
	if (this->objects.size() >= 1) {
		if (!this->objects.back()->undo()) {
			delete this->objects.back();
			this->objects.pop_back();
		}
	}
}

void canvas::dump(std::ostream& ost) const {
	ost << "canvas:{"
		<< this->objects
		<< "}";
}

void canvas::dump(std::ostream& ost, int tablevel) const {
	ost << tab*tablevel << "canvas:{" << std::endl;
	if (this->objects.size() > 0) {
		for (std::size_t i = 0; i < this->objects.size()-1; i++) {
			this->objects.at(i)->dump(ost, tablevel+1);
			ost << "," << std::endl;
		}
		this->objects.back()->dump(ost, tablevel+1);
		ost << std::endl;
	}
	ost << tab*tablevel << "}";
}

std::ostream& operator <<(std::ostream& ost, const canvas* canv) {
	canv->dump(ost);
	return ost;
}
