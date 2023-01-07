#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <GL/glut.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <FTGL/ftgl.h>

#include <nlohmann/json.hpp>

#include <vector>
#include <string>

#include "vec.hpp"
#include "object.hpp"

class canvas {
private:
	std::vector<object*> objects;
public:
	canvas();
	canvas(const nlohmann::json& json, FTPixmapFont* f);
	~canvas();
	void render(vec2<int> windowsize) const;
	void push_back(object* obj);
	void undo();
	nlohmann::json getjson() const;
};

canvas* create_canvas(lex_t& lex, FTPixmapFont* f);

#endif // include guard of CANVAS_HPP
