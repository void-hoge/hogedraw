#ifndef HOGEDRAW_HPP
#define HOGEDRAW_HPP

#include <GL/glut.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <FTGL/ftgl.h>

#include <vector>
#include <string>
#include <array>

#include "vec.hpp"
#include "object.hpp"
#include "canvas.hpp"
#include "lex.hpp"

class hogedraw {
private:
	vec2<int> windowsize;
	std::array<color_t, 6> colors;
	color_t background;
	color_t base;
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Renderer* renderer;
	FTPixmapFont* ftface;

	std::vector<canvas*> canvases;
	std::size_t current_canvas_idx;
	line* current_line;
	text* current_text;

	void init_opengl();

	void render() const;
	void dump(std::ostream& ost) const;

	void push_canvas();
	void move_to_next_canvas();
	void move_to_back_canvas();

	void handle_mouse_motion(const SDL_Event& event);
	void handle_mouse_press(const SDL_Event& event);
	void handle_mouse_release(const SDL_Event& event);
	void handle_window_events(const SDL_Event& event);
	void handle_text_input_event(const SDL_Event& event);
	bool handle_key_events(const SDL_Event& event);
public:
	hogedraw();
	hogedraw(lex_t& lex);
	~hogedraw();
	void mainloop();
};

hogedraw* create_hogedraw(lex_t& lex);

#endif // include guard of HOGEDRAW_HPP
