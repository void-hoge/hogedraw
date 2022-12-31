#include "hogedraw.hpp"
#include <iostream>

hogedraw* create_hogedraw(lex_t& lex) {
	if (lex.advance(std::regex("^root:"))) {
		return new hogedraw(lex);
	}else {
		throw std::runtime_error("syntax error at create_hogedraw");
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
	this->context = SDL_GL_CreateContext(this->window);
	if (!this->context) {
		throw std::runtime_error("Failed to init SDL OpenGL context.");
	}
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
	if (!this->renderer) {
		throw std::runtime_error("Failed to init SDL renderer.");
	}
	this->ftface = new FTPixmapFont("/home/hoge/.local/share/fonts/Roboto Mono for Powerline.ttf");
	if (this->ftface->Error()) {
		throw std::runtime_error("Failed to load the font.");
	}
	glClearColor((float)this->background.x()/255,
				 (float)this->background.y()/255,
				 (float)this->background.z()/255,
				 0.0);
	glViewport(0, 0, this->windowsize.x(), this->windowsize.y());
}

void hogedraw::render() const {
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT);
	this->canvases.at(this->current_canvas_idx)->render(this->windowsize);
	if (this->current_line != nullptr) {
		this->current_line->render(this->windowsize);
	}
	if (this->current_text != nullptr) {
		this->current_text->render(this->windowsize);
	}
	SDL_GL_SwapWindow(this->window);
}

void hogedraw::dump(std::ostream& ost) const {
	if (this->current_line != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_line);
	}
	if (this->current_text != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_text);
	}
	ost << "root:{"
		<< this->canvases
		<< "}";
}

void hogedraw::push_canvas() {
	this->canvases.push_back(new canvas());
}

void hogedraw::move_to_next_canvas() {
	if (this->current_text != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_text);
		this->current_text = nullptr;
	}
	if (this->current_line != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_line);
		this->current_line = nullptr;
	}
	this->current_canvas_idx = (this->current_canvas_idx+1)%this->canvases.size();
}

void hogedraw::move_to_back_canvas() {
	if (this->current_text != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_text);
		this->current_text = nullptr;
	}
	if (this->current_line != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_line);
		this->current_line = nullptr;
	}
	this->current_canvas_idx = this->canvases.size()-1;
}


void hogedraw::handle_mouse_motion(const SDL_Event& event) {
	if (this->current_text != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_text);
		this->current_text = nullptr;
	}
	if (event.motion.state | SDL_BUTTON_LMASK) {
		if (this->current_line != nullptr) {
			this->current_line->push_back(vec2<int>(event.motion.x, event.motion.y));
		}
	}
}

void hogedraw::handle_mouse_press(const SDL_Event& event) {
	if (event.button.button == SDL_BUTTON_LEFT) {
		if (this->current_line != nullptr) {
			this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_line);
			this->current_line = nullptr;
		}
		this->current_line = new line(this->base);
		this->current_line->push_back(vec2<int>(event.button.x, event.button.y));
	}
}

void hogedraw::handle_mouse_release(const SDL_Event& event) {
	if (event.button.button == SDL_BUTTON_LEFT) {
		if (this->current_line == nullptr) {
			return;
		}else {
			this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_line);
			this->current_line = nullptr;
		}
	}
}

void hogedraw::handle_window_events(const SDL_Event& event) {
	if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
		this->windowsize.x() = event.window.data1;
		this->windowsize.y() = event.window.data2;
		glViewport(0, 0, this->windowsize.x(), this->windowsize.y());
	}
}

void hogedraw::handle_text_input_event(const SDL_Event& event) {
	vec2<int> pos;
	SDL_GetMouseState(&(pos.x()), &(pos.y()));
	if (this->current_line != nullptr) {
		this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_line);
		this->current_line = nullptr;
	}
	if (this->current_text == nullptr) {
		this->current_text = new text(this->base, pos, this->ftface, 100);
		this->current_text->push_back(event.text.text[0]);
	}else {
		if (pos == this->current_text->pos()) {
			this->current_text->push_back(event.text.text[0]);
		}else {
			this->canvases.at(this->current_canvas_idx)->push_back((object*)this->current_text);
			this->current_text = new text(this->base, pos, this->ftface, 100);
			this->current_text->push_back(event.text.text[0]);
		}
	}
}

bool hogedraw::handle_key_events(const SDL_Event& event) {
	auto mod = event.key.keysym.mod;
	auto keycode = event.key.keysym.sym;
	if (mod & KMOD_CTRL) {
		if (keycode == SDLK_q) {
			return false;
		}else if (keycode == SDLK_TAB) {
			this->move_to_next_canvas();
		}else if (keycode == SDLK_t) {
			this->push_canvas();
			this->move_to_back_canvas();
		}else if (keycode == SDLK_s) {
			this->dump(std::cout);
		}
	} else if (mod == 0) {
		if (keycode == SDLK_BACKSPACE) {
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
	this->canvases.push_back(new canvas());
	this->current_canvas_idx = 0;
	this->current_line = nullptr;
	this->current_text = nullptr;
}

hogedraw::hogedraw(lex_t& lex) {
	this->colors.at(0) = color_t(0, 0, 0);
	this->colors.at(1) = color_t(255, 255, 255);
	this->background = this->colors.at(0);
	this->base = this->colors.at(1);
	this->init_opengl();
	if (!lex.advance(std::regex("\\{"))) {
		throw std::runtime_error("syntax error at hogedraw");
	}
	while (lex.check(std::regex("^canvas:"))) {
		this->canvases.push_back(create_canvas(lex, this->ftface));
		lex.advance(std::regex("^,"));
	}
	if (!lex.advance(std::regex("^\\}"))) {
		throw std::runtime_error("syntax error at hogedraw");
	}
	if (this->canvases.size() == 0) {
		this->canvases.push_back(new canvas());
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
		this->render();
	}
}
