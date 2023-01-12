#include "hogedraw.hpp"
#include "util.hpp"

#include <iostream>
#include <iomanip>

void hogedraw::init_font(const std::string fontpath) {
	this->ftface = new FTPixmapFont(fontpath.c_str());
	if (this->ftface->Error()) {
		throw std::runtime_error("Failed to load the font.");
	}
}

void hogedraw::init_opengl() {
	this->windowsize = vec2<int>(640, 480);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	this->window = SDL_CreateWindow(
		"hogedraw",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		this->windowsize.x(), this->windowsize.y(),
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!this->window) {
		throw std::runtime_error("Failed to init SDL window.");
	}
	SDL_SetWindowResizable(this->window, SDL_TRUE);
	this->context = SDL_GL_CreateContext(this->window);
	if (!this->context) {
		throw std::runtime_error("Failed to init SDL OpenGL context.");
	}
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
	if (!this->renderer) {
		throw std::runtime_error("Failed to init SDL renderer.");
	}
	glViewport(0, 0, this->windowsize.x(), this->windowsize.y());
}

void hogedraw::init_options(const option_t& option) {
	this->linethickness = option.linethickness;
	this->fontsize = option.fontsize;
	this->trianglefill = option.trianglefill;
	this->trianglesize = option.trianglesize;
	this->trianglethickness = option.trianglethickness;
	this->squarefill = option.squarefill;
	this->squaresize = option.squaresize;
	this->squarethickness = option.squarethickness;
	this->circlefill = option.circlefill;
	this->circlesize = option.circlesize;
	this->circlethickness = option.circlethickness;
	this->colors = option.colors;
	this->background = this->colors.at(0);
	this->base = this->colors.at(1);
	this->updated = true;
}

std::string hogedraw::get_time_string() {
	static const std::array<std::string, 12> monthstr = {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"June",
		"July",
		"Aug",
		"Sept",
		"Oct",
		"Nov",
		"Dec"
	};
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	auto timeptr = std::localtime(&time);
	auto year = timeptr->tm_year+1900;
	auto month = monthstr[timeptr->tm_mon];
	auto day = timeptr->tm_mday;
	auto hour = timeptr->tm_hour;
	auto minute = timeptr->tm_min;
	auto sec = timeptr->tm_sec;
	auto milli = std::chrono::duration_cast<std::chrono::milliseconds>
		(now.time_since_epoch()).count()%1000;
	std::stringstream sst;
	sst << year << "-"
		<< month << "-"
		<< std::setfill('0') << std::setw(2) << std::right << day << "-"
		<< std::setfill('0') << std::setw(2) << std::right << hour << ":"
		<< std::setfill('0') << std::setw(2) << std::right << minute << ":"
		<< std::setfill('0') << std::setw(2) << std::right << sec << ":"
		<< std::setfill('0') << std::setw(3) << std::right << milli;
	return sst.str();
}

void hogedraw::render() const {
	glMatrixMode(GL_PROJECTION);
	this->canvases.at(this->current_canvas_idx)->render(this->windowsize);
	if (this->current_line != nullptr) {
		this->current_line->render(this->windowsize);
	}
	if (this->current_text != nullptr) {
		this->current_text->render(this->windowsize);
	}
	SDL_GL_SwapWindow(this->window);
}

nlohmann::json hogedraw::getjson() {
	this->push_current_objects();
	nlohmann::json j;
	for (auto canvas: this->canvases) {
		j["canvases"].push_back(canvas->getjson());
	}
	return j;
}

void hogedraw::push_current_line() {
	if (this->current_line != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_line);
		this->current_line = nullptr;
	}
}

void hogedraw::push_current_text() {
	if (this->current_text != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_text);
		this->current_text = nullptr;
	}
}

void hogedraw::push_current_objects() {
	this->push_current_line();
	this->push_current_text();
}

bool hogedraw::export_window_as_png(std::string filename) {
	auto surface = SDL_GetWindowSurface(this->window);
	if (surface == nullptr) {
		throw std::runtime_error("failed to get window surface");
	}
	SDL_RenderReadPixels(this->renderer, NULL, SDL_GetWindowPixelFormat(this->window), surface->pixels, surface->pitch);
	auto retval = IMG_SavePNG(surface, filename.c_str());
	SDL_FreeSurface(surface);
	return retval == 0;
}

bool hogedraw::export_project_as_json_file(std::string filename) {
	std::ofstream ofst(filename);
	if (ofst.is_open()) {
		ofst << this->getjson().dump();
		ofst.close();
		return true;
	}
	return false;
}

void hogedraw::push_canvas() {
	this->canvases.push_back(new canvas(this->background));
}

vec2<int> hogedraw::get_mouse_pos() {
	vec2<int> pos;
	SDL_GetMouseState(&(pos.x()), &(pos.y()));
	return pos;
}

void hogedraw::switch_canvas(int num) {
	this->push_current_objects();
	auto tmp = ((int)this->current_canvas_idx+num)%(int)this->canvases.size();
	if (tmp < 0) {
		this->current_canvas_idx = tmp + this->canvases.size();
	}else {
		this->current_canvas_idx = tmp;
	}
}

void hogedraw::move_to_back_canvas() {
	this->push_current_objects();
	this->current_canvas_idx = this->canvases.size()-1;
}

void hogedraw::handle_mouse_motion(const SDL_Event& event) {
	if (this->current_text != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_text);
		this->current_text = nullptr;
	}
	if (event.motion.state & SDL_BUTTON_LMASK) {
		this->updated = true;
		if (this->current_line != nullptr) {
			this->current_line->push_back(vec2<int>(event.motion.x, event.motion.y));
		}
	}
}

void hogedraw::handle_mouse_press(const SDL_Event& event) {
	if (event.button.button == SDL_BUTTON_LEFT) {
		this->updated = true;
		if (this->current_line != nullptr) {
			this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_line);
			this->current_line = nullptr;
		}
		this->current_line = new line(this->base, this->linethickness);
		this->current_line->push_back(vec2<int>(event.button.x, event.button.y));
	}
}

void hogedraw::handle_mouse_release(const SDL_Event& event) {
	if (event.button.button == SDL_BUTTON_LEFT) {
		this->push_current_line();
	}
}

void hogedraw::handle_window_events(const SDL_Event& event) {
	this->updated = true;
	if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
		this->windowsize.x() = event.window.data1;
		this->windowsize.y() = event.window.data2;
		glViewport(0, 0, this->windowsize.x(), this->windowsize.y());
	}
}

void hogedraw::handle_text_input_event(const SDL_Event& event) {
	this->updated = true;
	vec2<int> pos = this->get_mouse_pos();
	this->push_current_line();
	if (this->current_text == nullptr) {
		this->current_text = new text(this->base, pos, this->ftface, this->fontsize);
		this->current_text->push_back(event.text.text[0]);
	}else {
		if (pos == this->current_text->pos()) {
			this->current_text->push_back(event.text.text[0]);
		}else {
			this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_text);
			this->current_text = new text(this->base, pos, this->ftface, this->fontsize);
			this->current_text->push_back(event.text.text[0]);
		}
	}
}

bool hogedraw::handle_key_events(const SDL_Event& event) {
	auto mod = event.key.keysym.mod;
	auto keycode = event.key.keysym.sym;
	static const auto ctrl_mask = KMOD_LCTRL | KMOD_RCTRL | KMOD_CTRL;
	static const auto shift_mask = KMOD_LSHIFT | KMOD_RSHIFT | KMOD_SHIFT;
	static const auto alt_mask = KMOD_LALT | KMOD_RALT | KMOD_ALT;
	if ((mod & ctrl_mask) && !(mod & shift_mask) && !(mod & alt_mask)) {
		if (keycode == SDLK_q) {
			return false;
		}else if (keycode == SDLK_TAB) {
			this->updated = true;
			this->switch_canvas(1);
		}else if (keycode == SDLK_t) {
			this->updated = true;
			this->push_canvas();
			this->move_to_back_canvas();
		}else if (keycode == SDLK_s) {
			std::string filename = "project"+this->get_time_string()+".json";
			this->export_project_as_json_file(filename);
		}else if (keycode == SDLK_p){
			std::string filename = "drawing"+this->get_time_string()+".png";
			this->export_window_as_png(filename);
		}else if (keycode == SDLK_r) {
			// triangle
			this->updated = true;
			auto pos = this->get_mouse_pos();
			this->canvases.at(this->current_canvas_idx)
				->push_back((object*)new regpoly(
								this->base,
								pos,
								3,
								this->trianglesize,
								this->trianglefill,
								this->trianglethickness));
		}else if (keycode == SDLK_f) {
			// rectangle
			this->updated = true;
			auto pos = this->get_mouse_pos();
			this->canvases.at(this->current_canvas_idx)
				->push_back((object*)new regpoly(
								this->base,
								pos, 4,
								this->squaresize,
								this->squarefill,
								this->squarethickness));
		}else if (keycode == SDLK_v) {
			// circle
			this->updated = true;
			auto pos = this->get_mouse_pos();
			this->canvases.at(this->current_canvas_idx)
				->push_back((object*)new regpoly(
								this->base,
								pos,
								16,
								this->circlesize,
								this->circlefill,
								this->circlethickness));
		}
		
	}else if ((mod & ctrl_mask) && (mod & shift_mask) && !(mod & alt_mask)) {
		if (keycode == SDLK_TAB) {
			this->updated = true;
			this->switch_canvas(-1);
		}
	}else if (mod == 0) {
		if (keycode == SDLK_BACKSPACE) {
			this->updated = true;
		    if (this->current_text != nullptr) {
				if (!this->current_text->undo()) {
					delete this->current_text;
					this->current_text = nullptr;
				}
			}else if (this->current_line != nullptr) {
				delete this->current_line;
				this->current_line = nullptr;
			}else {
				this->canvases.at(this->current_canvas_idx)->undo();
			}
		}else if (keycode == SDLK_F1) {
			this->push_current_objects();
			this->base = this->colors.at(1);
		}else if (keycode == SDLK_F2) {
			this->push_current_objects();
			this->base = this->colors.at(2);
		}else if (keycode == SDLK_F3) {
			this->push_current_objects();
			this->base = this->colors.at(3);
		}else if (keycode == SDLK_F4) {
			this->push_current_objects();
			this->base = this->colors.at(4);
		}else if (keycode == SDLK_F5) {
			this->push_current_objects();
			this->base = this->colors.at(5);
		}

	}
	return true;
}

hogedraw::hogedraw() {
	this->colors.at(0) = color_t(0, 0, 0);
	this->colors.at(1) = color_t(255, 255, 255);
	this->background = this->colors.at(0);
	this->base = this->colors.at(1);
	this->init_opengl();
	this->init_font();
	this->canvases.push_back(new canvas(this->background));
	this->current_canvas_idx = 0;
	this->current_line = nullptr;
	this->current_text = nullptr;
	this->linethickness = 2;
	this->trianglefill = false;
	this->trianglesize = 50;
	this->trianglethickness = 2;
	this->squarefill = false;
	this->squaresize = 50;
	this->squarethickness = 2;
	this->circlefill = false;
	this->circlesize = 50;
	this->circlethickness = 2;
	this->fontsize = 50;
}

hogedraw::hogedraw(const option_t& option) {
	this->init_options(option);
	this->init_opengl();
	this->init_font(option.fontpath);
	this->canvases.push_back(new canvas(this->background));
	this->current_canvas_idx = 0;
	this->current_line = nullptr;
	this->current_text = nullptr;
}

hogedraw::hogedraw(const nlohmann::json& json, const option_t& option) {
	this->init_options(option);
	this->init_opengl();
	this->init_font(option.fontpath);
	for (std::size_t i = 0; i < json["canvases"].size(); i++) {
		this->canvases.push_back(new canvas(json["canvases"].at(i), this->ftface));
	}
	if (this->canvases.size() == 0) {
		this->canvases.push_back(new canvas(this->background));
	}
	this->current_canvas_idx = 0;
	this->current_line = nullptr;
	this->current_text = nullptr;
}


hogedraw::~hogedraw() {
	for (auto canv: this->canvases) {
		delete canv;
	}
	this->canvases.clear();
	SDL_Quit();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	delete this->current_text;
	delete this->current_line;
	this->current_text = nullptr;
	this->current_line = nullptr;
	SDL_DestroyRenderer(renderer);
	this->renderer = nullptr;
}

void hogedraw::mainloop() {
	SDL_Event event;
	SDL_StartTextInput();
	while (SDL_WaitEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return;
		}else if (event.type == SDL_MOUSEMOTION) {
			this->handle_mouse_motion(event);
		}else if (event.type == SDL_MOUSEBUTTONDOWN) {
			this->handle_mouse_press(event);
		}else if (event.type == SDL_MOUSEBUTTONUP) {
			this->handle_mouse_release(event);
		}else if (event.type == SDL_WINDOWEVENT) {
			this->handle_window_events(event);
		}else if (event.type == SDL_TEXTINPUT) {
			this->handle_text_input_event(event);
		}else if (event.type == SDL_KEYDOWN) {
			if (!this->handle_key_events(event)) {
				return;
			}
		}
		if (this->updated) {
			this->render();
			this->updated = false;
		}
	}
}
