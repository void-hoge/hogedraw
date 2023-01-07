#include "vec.hpp"
#include "object.hpp"
#include "util.hpp"

#include <cmath>
#include <iostream>

object* create_object(const nlohmann::json& json, FTPixmapFont* f) {
	if (json["type"] == "line") {
		return (object*)new line(json);
	}else if (json["type"] == "text") {
		return (object*)new text(json, f);
	}else if (json["type"] == "regpoly") {
		return (object*)new regpoly(json);
	}else {
		throw std::invalid_argument("Invalid object type.");
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

line::line(const nlohmann::json& json) {
	this->color = vec3<int>(json["color"]);
	this->thickness = json["thickness"];
	for (std::size_t i = 0; i < json["vertices"].size(); i++) {
		this->vertices.push_back(vec2<int>(json["vertices"].at(i)));
	}
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

nlohmann::json line::getjson() const {
	nlohmann::json j;
	j["type"] = "line";
	j["color"] = this->color.getjson();
	j["thickness"] = this->thickness;
	for (const auto& vertex: this->vertices) {
		j["vertices"].push_back(vertex.getjson());
	}
	return j;
}

void line::push_back(const vec2<int>& pos) {
	this->vertices.push_back(pos);
}

text::text(const color_t& c, const vec2<int>& p, FTPixmapFont* f, int s, const std::string& init)  {
	this->color = c;
	this->pos_ = p;
	this->ftface = f;
	this->size = s;
	this->str = init;
}

text::text(const nlohmann::json& json, FTPixmapFont* f) {
	this->ftface = f;
	this->color = vec3<int>(json["color"]);
	this->pos_ = vec2<int>(json["pos"]);
	this->size = json["size"];
	this->str = json["str"];
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

nlohmann::json text::getjson() const {
	nlohmann::json j;
	j["type"] = "text";
	j["color"] = this->color.getjson();
	j["pos"] = this->pos().getjson();
	j["size"] = this->size;
	j["str"] = this->str;
	return j;
}

const vec2<int>& text::pos() const {
	return this->pos_;
}

regpoly::regpoly(const color_t& c, const vec2<int>& p, int n, int s, bool f, int t) {
	this->color = c;
	this->pos = p;
	this->n = n;
	this->size = s;
	this->filled = f;
	this->thickness = t;
}

regpoly::regpoly(const nlohmann::json& json) {
	this->color = vec3<int>(json["color"]);
	this->pos = vec2<int>(json["pos"]);
	this->n = json["n"];
	this->size = json["size"];
	this->filled = json["filled"];
	this->thickness = json["thickness"];
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

nlohmann::json regpoly::getjson() const {
	nlohmann::json j;
	j["type"] = "regpoly";
	j["color"] = this->color.getjson();
	j["pos"] = this->pos.getjson();
	j["n"] = this->n;
	j["size"] = this->size;
	j["filled"] = this->filled;
	j["thickness"] = this->thickness;
	return j;
}
