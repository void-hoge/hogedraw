#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <GL/glut.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <FTGL/ftgl.h>

#include <vector>
#include <string>

#include "vec.hpp"
#include "object.hpp"
#include "lex.hpp"


class canvas {
private:
	std::vector<object*> objects;
public:
	canvas();
	canvas(lex_t& lex, FTPixmapFont* f);
	~canvas();
	void render(vec2<int> windowsize) const;
	void push_back(object* obj);
	void undo();
	void dump(std::ostream& ost) const;
	void dump(std::ostream& ost, int tablevel) const;
};

canvas* create_canvas(lex_t& lex, FTPixmapFont* f);

std::ostream& operator <<(std::ostream& ost, const canvas* canv);

#endif // include guard of CANVAS_HPP
