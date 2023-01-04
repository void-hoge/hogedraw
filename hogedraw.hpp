#ifndef HOGEDRAW_HPP
#define HOGEDRAW_HPP

#include <GL/glut.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <FTGL/ftgl.h>

#include <vector>
#include <string>
#include <array>
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>

#include "vec.hpp"
#include "object.hpp"
#include "canvas.hpp"
#include "lex.hpp"
#include "option.hpp"

class hogedraw {
private:
	vec2<int> windowsize;

	SDL_Window* window;
	SDL_GLContext context;
	SDL_Renderer* renderer;
	FTPixmapFont* ftface;

	std::array<color_t, 6> colors;
	color_t background;
	color_t base;
	int fontsize;
	int linethickness;
	bool trianglefill;
	int trianglesize;
	int trianglethickness;
	bool squarefill;
	int squaresize;
	int squarethickness;
	bool circlefill;
	int circlesize;
	int circlethickness;

	std::vector<canvas*> canvases;
	std::size_t current_canvas_idx;
	line* current_line;
	text* current_text;

	bool updated;

	void init_font(const std::string fontpath="/usr/share/fonts/truetype/freefont/FreeMono.ttf");
	void init_opengl();
	void init_options(const option_t& option);

	std::string get_time_string();
	bool export_window_as_png(std::string filename);
	bool export_project_as_text_file(std::string filename);

	void render() const;
	void dump(std::ostream& ost, int tablevel);
	void dump(std::ostream& ost);

	void push_current_line();
	void push_current_text();
	void push_current_objects();

	void push_canvas();
	void switch_canvas(int num);
	void move_to_back_canvas();

	vec2<int> get_mouse_pos();

	void handle_mouse_motion(const SDL_Event& event);
	void handle_mouse_press(const SDL_Event& event);
	void handle_mouse_release(const SDL_Event& event);
	void handle_window_events(const SDL_Event& event);
	void handle_text_input_event(const SDL_Event& event);
	bool handle_key_events(const SDL_Event& event);
public:
	hogedraw();
	hogedraw(lex_t& lex);
	hogedraw(const option_t& option);
	hogedraw(lex_t& lex, const option_t& option);
	~hogedraw();
	void mainloop();
};

hogedraw* create_hogedraw(lex_t& lex, option_t& option);

#endif // include guard of HOGEDRAW_HPP
