#include "common.h"
#include <cmath>

const Num EPS = 0.000001;

bool equal(Num f0, Num f1) {
	if(f0>f1) std::swap(f0,f1);
	Num dif = f1-f0;
	return dif < EPS || dif < f1 * EPS;
}

bool point_in_rect(Point p, Rect rect) {
	if(p.x < rect.l || p.x > rect.r)
		return false;
	if(p.y < rect.b || p.y > rect.t)
		return false;
	return (p.x > rect.l && p.x < rect.r)
		&& (p.y > rect.b && p.y < rect.t);
}

bool invalid_rect(Rect rect, Num X, Num Y) {
	if(rect.area < 0)
		return true;
	if(rect.t < rect.b || rect.l > rect.r)
		return true;
	if(rect.l < 0 || rect.r > X)
		return true;
	if(rect.b < 0 || rect.t > Y)
		return true;
	return false;
}

int main(int argc, char* argv[]) {
	Num X, Y;
	size_t N;
	std::vector<Point> points;

	std::ifstream input(argv[1]);
	read_input(input, X, Y, N, points);

	Num expected_answer;
	std::ifstream expected(argv[2]);
	expected >> expected_answer;

	Rect output_rect;
	std::ifstream actual(argv[3]);
	actual >> output_rect;

	if(output_rect.area != expected_answer) {
		std::cout << "Wrong area" << std::endl;
		std::cout << "Expected: " << expected_answer << std::endl;
		std::cout << "Got: " << output_rect.area << std::endl;
		Num dif = expected_answer - output_rect.area;
		std::cout << "Absolute difference is " << dif << std::endl;
		std::cout << "Relative difference is " << fabs(dif) / std::max(expected_answer, output_rect.area) << std::endl;
		std::cout << "Rect is " << output_rect << std::endl;
		return EXIT_FAILURE;
	}

	if(invalid_rect(output_rect, X, Y)) {
		std::cout << "Invalid rect: " << output_rect << std::endl;
		std::cout << "Area is: " << output_rect.area << std::endl;
		std::cout << "X = " << X << " and Y = " << Y << std::endl;
		return EXIT_FAILURE;
	}

	for(size_t i=0; i<N; ++i) {
		Point p = points[i];
		if(point_in_rect(p, output_rect)) {
			std::cout << "The rect contain forbidden point" << std::endl;
			std::cout << "Rect is " << output_rect << std::endl;
			std::cout << "Forbidden point is " << p << std::endl;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
