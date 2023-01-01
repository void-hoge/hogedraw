#ifndef VEC_HPP
#define VEC_HPP

#include <ostream>
#include <vector>

template<typename T>
class vec2 {
private:
	T x_;
	T y_;
public:
	vec2(): x_(0), y_(0){}
	vec2(T x, T y): x_(x), y_(y) {}
	vec2(const vec2<T>& vec): x_(vec.x()), y_(vec.y()) {}
	T& x() {
		return this->x_;
	}
	T& y() {
		return this->y_;
	}
	const T& x() const {
		return this->x_;
	}
	const T& y() const {
		return this->y_;
	}
	void dump(std::ostream& ost) const {
		ost << "("
			<< this->x()
			<< ","
			<< this->y()
			<< ")";
	}
};

template<typename T>
std::ostream& operator <<(std::ostream& ost, const vec2<T>& vec) {
	vec.dump(ost);
	return ost;
}

template<typename T>
bool operator ==(const vec2<T>& a, const vec2<T>& b) {
	return a.x() == b.x() && a.y() == b.y();
}

template<typename T>
class vec3 {
private:
	T x_;
	T y_;
	T z_;
public:
	vec3(): x_(0), y_(0), z_(0){}
	vec3(T x, T y, T z): x_(x), y_(y), z_(z) {}
	vec3(const vec3<T>& vec): x_(vec.x()), y_(vec.y()), z_(vec.z()) {}
	T& x() {
		return this->x_;
	}
	T& y() {
		return this->y_;
	}
	T& z() {
		return this->z_;
	}
	const T& x() const {
		return this->x_;
	}
	const T& y() const {
		return this->y_;
	}
	const T& z() const {
		return this->z_;
	}
	void dump(std::ostream& ost) const {
		ost << "("
			<< this->x()
			<< ","
			<< this->y()
			<< ","
			<< this->z()
			<< ")";
	}
};

using color_t = vec3<int>;

template<typename T>
std::ostream& operator <<(std::ostream& ost, const vec3<T> vec) {
	vec.dump(ost);
	return ost;
}

template<typename T>
bool operator ==(const vec3<T>& a, const vec3<T>& b) {
	return a.x() == b.x() && a.y() == b.y() && a.z() == b.z();
}

template<typename T>
std::ostream& operator <<(std::ostream& ost, const std::vector<T> vec) {
	if (vec.size() > 0) {
		for (std::size_t i = 0; i < vec.size()-1; i++) {
			ost << vec.at(i) << ",";
		}
		ost << vec.back();
	}
	return ost;
}

#endif // include guard of VEC_HPP