#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <GL/glut.h>
#include <SDL.h>
#include <SOIL/SOIL.h>
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
	color_t background;
public:
	canvas(color_t bg);
	canvas(color_t bg, const std::string& fn);
	canvas(const nlohmann::json& json, FTPixmapFont* f);
	~canvas();
	void loadimage(std::string flnm);
	void render(const vec2<int>& offset, const double scale) const;
	void push_back(object* obj);
	void undo();
	nlohmann::json getjson() const;
};

canvas* create_canvas(lex_t& lex, FTPixmapFont* f);

#endif // include guard of CANVAS_HPP
