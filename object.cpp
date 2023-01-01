#include "vec.hpp"
#include "object.hpp"

#include <cmath>
#include <iostream>

object* create_object(lex_t& lex, FTPixmapFont* f) {
	if (!lex.check(std::regex("^(text|line|regpoly):"))) {
		throw std::runtime_error("syntax error at create_object");
	}
	if (lex.match().str() == "text:") {
		lex.advance();
		return (object*)new text(lex, f);
	}else if (lex.match().str() == "line:") {
		lex.advance();
		return (object*)new line(lex);
	}else {
		lex.advance();
		return (object*)new regpoly(lex);
	}
}

std::ostream& operator <<(std::ostream& ost, const object* obj) {
	obj->dump(ost);
	return ost;
}

line::line(lex_t& lex) {
	if (!lex.advance(std::regex("^\\{"))) {
		throw std::runtime_error("syntax error at line");
	}

	if (lex.check(std::regex("^color:\\((\\d+),(\\d+),(\\d+)\\),"))) {
		this->color.x() = std::stoi(lex.match().str(1));
		this->color.y() = std::stoi(lex.match().str(2));
		this->color.z() = std::stoi(lex.match().str(3));
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at line, while parsing the color");
	}

	if (lex.check(std::regex("^thickness:(\\d+),"))) {
		this->thickness = std::stoi(lex.match().str(1));
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at line, while parsing the thickness");
	}

	if (lex.advance(std::regex("^vertices:\\{"))) {
		while (lex.check(std::regex("^\\((\\d+),(\\d+)\\)"))) {
			this->vertices.push_back(vec2<int>(
										 std::stoi(lex.match().str(1)),
										 std::stoi(lex.match().str(2))
										 ));
			lex.advance();
			lex.advance(std::regex("^,"));
		}
		if (!lex.advance(std::regex("^\\}"))) {
			throw std::runtime_error("syntax error at line, while parsing the vertices");
		}
	}else {
		throw std::runtime_error("syntax error at line, while parsing the vertices");
	}

	if (!lex.advance(std::regex("^\\}"))) {
		throw std::runtime_error("syntax error at line");
	}
}

line::line(const color_t& c, int t) {
	this->color = c;
	this->thickness = t;
	
}

line::line(const color_t& c, int t, const std::vector<vec2<int>>& init) {
	this->color = c;
	this->thickness = t;
	this->vertices = init;
}

void line::render(const vec2<int>& windowsize) const {
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, windowsize.x(), windowsize.y(), 0, 0, 1);
	glColor3d(this->color.x(), this->color.y(), this->color.z());
	glLineWidth(this->thickness);
	glBegin(GL_LINE_STRIP);
	for (auto point: this->vertices) {
		glVertex2f(point.x(), point.y());
	}
	glEnd();
	glPopMatrix();
}

bool line::undo() {
	return false;
}

void line::dump(std::ostream& ost) const {
	ost << "line:{"
		<< "color:" << this->color << ","
		<< "thickness:" << this->thickness << ","
		<< "vertices:{" << this->vertices << "}"
		<< "}";
}

void line::push_back(const vec2<int>& pos) {
	this->vertices.push_back(pos);
}

text::text(lex_t& lex, FTPixmapFont* f) {
	this->ftface = f;
	if (!lex.advance(std::regex("^\\{"))) {
		throw std::runtime_error("syntax error at text");
	}
	if (lex.check(std::regex("^color:\\((\\d+),(\\d+),(\\d+)\\),"))) {
		this->color.x() = std::stoi(lex.match().str(1));
		this->color.y() = std::stoi(lex.match().str(2));
		this->color.z() = std::stoi(lex.match().str(3));		
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at text, while parsing the color");
	}
	
	if (lex.check(std::regex("^pos:\\((\\d+),(\\d+)\\),"))) {
		this->pos_.x() = std::stoi(lex.match().str(1));
		this->pos_.y() = std::stoi(lex.match().str(2));
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at text, while parsing pos");
	}

	if (lex.check(std::regex("^size:(\\d+),"))) {
		this->size = std::stoi(lex.match().str(1));
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at text, while parsing size");
	}

	std::size_t len;
	if (lex.check(std::regex("^len:(\\d+),"))) {
		len = std::stoi(lex.match().str(1));
		lex.advance();
	}
	std::string re = std::string("^str:\"(") + std::string(".")*len + ")\"";
	if (lex.check(std::regex(re))) {
		this->str = lex.match().str(1);
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at text, while parsing str");
	}
	if (!lex.advance(std::regex("^\\}"))) {
		throw std::runtime_error("syntax error at text");
	}
}

text::text(const color_t& c, const vec2<int>& p, FTPixmapFont* f, int s, const std::string& init)  {
	this->color = c;
	this->pos_ = p;
	this->ftface = f;
	this->size = s;
	this->str = init;
}

void text::render(const vec2<int>& windowsize) const {
	glPushMatrix();
	glLoadIdentity();
	this->ftface->FaceSize(this->size);
	glOrtho(0, windowsize.x(), windowsize.y(), 0, 0, 1);
	glColor3d(this->color.x(), this->color.y(), this->color.z());
	glRasterPos2f(this->pos().x(), this->pos().y()+(this->ftface->LineHeight()));
	this->ftface->Render(this->str.c_str());
	glPopMatrix();
}

bool text::undo() {
	if (this->str.size() <= 1) {
		this->str.pop_back();
		return false;
	}else {
		this->str.pop_back();
		return true;
	}
}

void text::push_back(const char ch) {
	this->str.push_back(ch);
}

void text::dump(std::ostream& ost) const {
	ost << "text:{"
		<< "color:" << this->color << ","
		<< "pos:" << this->pos_ << ","
		<< "size:" << this->size << ","
		<< "len:" << this->str.size() << ","
		<< "str:\"" << this->str << "\""
		<< "}";
}

const vec2<int>& text::pos() const {
	return this->pos_;
}

regpoly::regpoly(lex_t& lex) {
	if (!lex.advance(std::regex("^\\{"))) {
		throw std::runtime_error("syntax error at regpoly");
	}

	if (lex.check(std::regex("^color:\\((\\d+),(\\d+),(\\d+)\\),"))) {
		this->color.x() = std::stoi(lex.match().str(1));
		this->color.y() = std::stoi(lex.match().str(2));
		this->color.z() = std::stoi(lex.match().str(3));
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at regpoly, while parsing the color");
	}

	if (lex.check(std::regex("^pos:\\((\\d+),(\\d+)\\),"))) {
		this->pos.x() = std::stoi(lex.match().str(1));
		this->pos.y() = std::stoi(lex.match().str(2));
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at regpoly, while parsing the pos");
	}

	if (lex.check(std::regex("^n:(\\d+),"))) {
		this->n = std::stoi(lex.match().str(1));
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at regpoly, while parsing the n");
	}

	if (lex.check(std::regex("^size:(\\d+),"))) {
		this->size = std::stoi(lex.match().str(1));
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at regpoly, while parsing the size");
	}

	if (lex.check(std::regex("^filled:(true|false),"))) {
		if (lex.match().str(1) == "true") {
			this->filled = true;
		}else {
			this->filled = false;
		}
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at regpoly, while parsing the filled");
	}

	if (lex.check(std::regex("^thickness:(\\d+)"))) {
		this->thickness = std::stoi(lex.match().str(1));
		lex.advance();
	}else {
		throw std::runtime_error("syntax error at regpoly, while parsing the thickness");
	}
	
	if (!lex.advance(std::regex("^\\}"))) {
		throw "syntax error at regpoly";
	}
}

regpoly::regpoly(const color_t& c, const vec2<int>& p, int n, int s, bool f, int t) {
	this->color = c;
	this->pos = p;
	this->n = n;
	this->size = s;
	this->filled = f;
	this->thickness = t;
}

void regpoly::render(const vec2<int>& windowsize) const {
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, windowsize.x(), windowsize.y(), 0, 0, 1);
	glColor3d(this->color.x(), this->color.y(), this->color.z());
	glLineWidth(this->thickness);
	if (this->filled) {
		glBegin(GL_TRIANGLE_FAN);
	}else {
		glBegin(GL_LINE_LOOP);
	}
	double delta = (double)(2*PI)/this->n;
	for (double theta = delta/2; theta < PI*2; theta+=delta) {
		float x = this->pos.x()-this->size*std::sin(theta);
		float y = this->pos.y()+this->size*std::cos(theta);
		glVertex2f(x, y);
	}
	glEnd();
	glPopMatrix();	
}

bool regpoly::undo() {
	return false;
}

void regpoly::dump(std::ostream& ost) const {
	ost << std::boolalpha
		<< "regpoly:{"
		<< "color:" << this->color << ","
		<< "pos:" << this->pos << ","
		<< "n:" << this->n << ","
		<< "size:" << this->size << ","
		<< "filled:" << this->filled << ","
		<< "thickness:" << this->thickness
		<< "}";
}
