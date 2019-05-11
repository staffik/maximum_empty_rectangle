#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <iomanip>

using Num = long double;
using Num_l = std::numeric_limits<Num>;

struct Point {
	Num x, y;

	Point() : x(0), y(0) {}
	Point(Num x, Num y) : x(x), y(y) {}
	Point(const Point&) = default;
	bool operator==(const Point &p) const {
		return x == p.x && y == p.y;
	}
};

std::istream& operator>>(std::istream &is, Point &p) {
	return is >> p.x >> p.y;
}

std::ostream& operator<<(std::ostream &os, const Point &p) {
	return os << std::setprecision(Num_l::max_digits10) << p.x << " " << p.y;
}

struct Rect {
	Num t, l, b, r;
	Num area;

	Rect() : t(0), l(0), b(0), r(0), area(0) {}

	Rect(Num t, Num l, Num b, Num r) : t(t), l(l), b(b), r(r) {
		area = (t-b) * (r-l);
	}
};

std::istream &operator>>(std::istream &is, Rect &rect) {
	Num t, l, b, r;
	is >> t >> l >> b >> r;
	rect = Rect(t,l,b,r);
	return is;
}

std::ostream& operator<<(std::ostream &os, const Rect &rect) {
	return os << std::setprecision(Num_l::max_digits10) << rect.t << " " << rect.l << " " << rect.b << " " << rect.r;
	//return os << "[("<<rect.l << "," << rect.t << "),(" << rect.r << "," << rect.b<<"): "<<rect.area<<"]";

}

void update(Rect &max_rect, Rect new_rect) {
	if(new_rect.area > max_rect.area) {
		max_rect = new_rect;
	}
}

void read_input(std::istream &is, Num &X, Num &Y, size_t &N, std::vector<Point> &points) {
	std::ios::sync_with_stdio(0);
	is >> X >> Y >> N;
	points.resize(N);
	for(size_t i=0; i<N; ++i) {
		is >> points[i] >> points[i];
	}
}


void read_input(std::istream &is, Num &X, Num &Y, size_t &N, std::vector<Rect> &rects) {
	std::ios::sync_with_stdio(0);
	is >> X >> Y >> N;
	rects.resize(N);
	for(size_t i=0; i<N; ++i) {
		Point tl, br;
		is >> tl >> br;
		rects[i] = Rect(tl.y, tl.x, br.y, br.x);
	}
}
