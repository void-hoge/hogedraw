#include "canvas.hpp"
#include "util.hpp"
#include <iostream>

canvas::canvas() {}

canvas::canvas(const nlohmann::json& json, FTPixmapFont* f) {
	for (std::size_t i = 0; i < json["objects"].size(); i++) {
		this->objects.push_back(create_object(json["objects"].at(i), f));
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

nlohmann::json canvas::getjson() const {
	nlohmann::json j;
	for (auto object: this->objects) {
		j["objects"].push_back(object->getjson());
	}
	return j;
}
