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
	}else if (json["type"] == "image") {
		return (object*)new image(json);
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
	nlohmann::json json;
	json["type"] = "line";
	json["color"] = this->color.getjson();
	json["thickness"] = this->thickness;
	for (const auto& vertex: this->vertices) {
		json["vertices"].push_back(vertex.getjson());
	}
	return json;
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
	nlohmann::json json;
	json["type"] = "text";
	json["color"] = this->color.getjson();
	json["pos"] = this->pos().getjson();
	json["size"] = this->size;
	json["str"] = this->str;
	return json;
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
	nlohmann::json json;
	json["type"] = "regpoly";
	json["color"] = this->color.getjson();
	json["pos"] = this->pos.getjson();
	json["n"] = this->n;
	json["size"] = this->size;
	json["filled"] = this->filled;
	json["thickness"] = this->thickness;
	return json;
}

image::image(const nlohmann::json& json) {
	this->pos = vec2<int>(json["pos"]);
	this->filename = json["filename"];
	this->size = vec2<int>(json["size"]);
	this->loadimage(this->filename);
}

image::image(const std::string& fn, const vec2<int>& p) {
	this->filename = fn;
	this->pos = p;
	this->loadimage(this->filename);
}

void image::loadimage(const std::string& fn) {
	int channels;
	unsigned char* image = SOIL_load_image(
		fn.c_str(),
		&(this->size.x()),
		&(this->size.y()),
		&channels,
		SOIL_LOAD_AUTO);
	if (image == nullptr) {
		std::string err = "Failed to load \"" + fn + "\".";
		throw std::invalid_argument(err);
	}
	if (channels == 4) {
		this->format = GL_RGBA;
	}else {
		this->format = GL_RGB;
	}
	glGenTextures(1, &(this->id));
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(
		GL_TEXTURE_2D,
		0, this->format,
		this->size.x(), this->size.y(),
		0, this->format,
		GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void image::render(const vec2<int>& windowsize) const {
	glPushMatrix();
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, this->id);
	glColor3d(255, 255, 255);
	glEnable(GL_TEXTURE_2D);
	glOrtho(0, windowsize.x(), windowsize.y(), 0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 1.0);
	glVertex3d(0.0, this->size.y(), 0.0);
	glTexCoord2d(1.0, 1.0);
	glVertex3d(this->size.x(), this->size.y(), 0.0);
	glTexCoord2d(1.0, 0.0);
	glVertex3d(this->size.x(), 0.0,  0.0);
	glTexCoord2d(0.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

bool image::undo() {
	return false;
}

nlohmann::json image::getjson() const {
	nlohmann::json json;
	json["type"] = "image";
	json["pos"] = this->pos.getjson();
	json["size"] = this->size.getjson();
	json["filename"] = this->filename;
	return json;
}
