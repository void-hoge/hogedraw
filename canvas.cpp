#include "canvas.hpp"
#include <iostream>

canvas::canvas(color_t bg): background(bg) {}

canvas::canvas(color_t bg, const std::string& fn): background(bg) {
	this->objects.push_back((object*)new image(fn, vec2<int>(0,0)));
}

canvas::canvas(const nlohmann::json& json, FTPixmapFont* f) {
	this->background = color_t(json["background"]["color"]);
	for (std::size_t i = 0; i < json["objects"].size(); i++) {
		this->objects.push_back(create_object(json["objects"].at(i), f));
	}
}

canvas::~canvas() {}

void canvas::render(const vec2<int>& offset, const double scale) const {
	glClearColor((float)this->background.x()/255,
				 (float)this->background.y()/255,
				 (float)this->background.z()/255,
				 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	for (auto obj: this->objects) {
		obj->render(offset, scale);
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

nlohmann::json canvas::getjson() const {
	nlohmann::json json;
	json["background"]["color"] = this->background.getjson();
	for (auto object: this->objects) {
		json["objects"].push_back(object->getjson());
	}
	return json;
}
