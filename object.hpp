#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <GL/glut.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <FTGL/ftgl.h>

#include <nlohmann/json.hpp>

#include <vector>
#include <string>

#include "vec.hpp"
#include "lex.hpp"

const double PI = 3.14159265;

class object{
protected:
	color_t color;
public:
	virtual void render(const vec2<int>& windowsize) const = 0;
	virtual bool undo() = 0;
	virtual nlohmann::json getjson() const = 0;
};

object* create_object(const nlohmann::json& json, FTPixmapFont* f);

std::ostream& operator<<(std::ostream& ost, const object* obj);

class line : protected object {
private:
	std::vector<vec2<int>> vertices;
	int thickness;
public:
	line(const nlohmann::json& json);
	line(const color_t& c, int t=2);
	line(const color_t& c, int t, const std::vector<vec2<int>>& init);
	void render(const vec2<int>& windowsize) const;
	bool undo();
	nlohmann::json getjson() const;
	void push_back(const vec2<int>& pos);
};

class text : protected object {
private:
	vec2<int> pos_;
	FTPixmapFont* ftface;
	int size;
	std::string str;
public:
	text(const nlohmann::json& json, FTPixmapFont* f);
	text(const color_t& c, const vec2<int>& p, FTPixmapFont* f, int s=100, const std::string& init="");
	void render(const vec2<int>& windowsize) const;
	bool undo();
	void push_back(const char ch);
	nlohmann::json getjson() const;
	const vec2<int>& pos() const;
};

class regpoly : protected object {
private:
	vec2<int> pos;
	int n;
	int size;
	bool filled;
	int thickness;
public:
	regpoly(const nlohmann::json& json);
	regpoly(const color_t& c, const vec2<int>& p, int n, int s=100, bool f=false, int t=2);
	void render(const vec2<int>& windowsize) const;
	bool undo();
	nlohmann::json getjson() const;
};

#endif // include guard of OBJECT_HPP
