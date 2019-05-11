#include "common.h"
#include <random>

int main(int argc, char* argv[]) {
	const Num X = atof(argv[1]);
	const Num Y = X;
	size_t N = atol(argv[2]);
	const int method = atoi(argv[3]);

	std::random_device rd;
	std::mt19937 eng(rd());

	std::vector<Point> points(N);
	auto fill_points = [&](auto gen_X, auto gen_Y) {
		for(size_t i=0; i<N; ++i) {
			points[i].x = gen_X(eng);
			points[i].y = gen_Y(eng);
		}
	};

	switch(method) {
		case -1: {
			std::uniform_int_distribution<int> distr_X(0,X);
			std::uniform_int_distribution<int> distr_Y(0,Y);
			fill_points(distr_X, distr_Y);
			break;
		}
		case 0: {
			std::uniform_real_distribution<Num> distr_X(0,X);
			std::uniform_real_distribution<Num> distr_Y(0,Y);
			fill_points(distr_X, distr_Y);
			break;
		}
		case 1: {
			std::normal_distribution<Num> distr_X(X/2,X/4);
			std::normal_distribution<Num> distr_Y(Y/2,Y/4);
			fill_points(distr_X, distr_Y);
			break;
		}
		case 2: {
			std::uniform_real_distribution<Num> distr_X(0,X);
			std::normal_distribution<Num> distr_Y(Y/2,Y/4);
			fill_points(distr_X, distr_Y);
			break;
		}
		default: {
			std::cerr << "Unrecognized generating method" << std::endl;
			return EXIT_FAILURE;
		}
	}

	for(auto& p : points) {
		p.x = std::min(std::max((Num)0.0,p.x),X);
		p.y = std::min(std::max((Num)0.0,p.y),Y);
	}

	auto points_cmp = [](auto& p0, auto& p1) {
		return p0.x == p1.x ? p0.y<p1.y : p0.x<p1.x;
	};
	std::sort(points.begin(), points.end(), points_cmp);
	auto it = std::unique(points.begin(), points.end());
	N = it - points.begin();
	points.resize(N);
	std::random_shuffle(points.begin(), points.end());

	std::cout << std::setprecision(Num_l::max_digits10) << X << " " << Y << " " << N << std::endl;
	for(auto& p : points) {
		std::cout << std::setprecision(Num_l::max_digits10) << p << " " << p << "\n";
	}
	
	return EXIT_SUCCESS;
}
