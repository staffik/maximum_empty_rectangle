#include "common.h"

int main(int argc, char* argv[]) {
	Num X, Y;
	size_t N;
	std::vector<Point> points;

	read_input(std::cin, X, Y, N, points);

	Rect max_rect;

	auto ascending_X_order = [](const Point &p0, const Point &p1){
		return p0.x < p1.x;
	};
	std::sort(points.begin(), points.end(), ascending_X_order);
	update(max_rect, Rect(Y, 0, 0, points[0].x));
	for(size_t i=1; i<N; ++i) {
		update(max_rect, Rect(Y, points[i-1].x, 0, points[i].x));
	}
	update(max_rect, Rect(Y, points[N-1].x, 0, X));

	auto descending_Y_order = [](const Point &p0, const Point &p1){
		return p0.y > p1.y;
	};
	std::sort(points.begin(), points.end(), descending_Y_order);
	for(size_t i=0; i<N; ++i) {
		Point top = points[i];
		Num max_left = 0, min_right = X;
		for(size_t j=i+1; j<N; ++j) {
			Point bottom = points[j];
			if(bottom.x < max_left || bottom.x > min_right) {
				continue;
			}
			update(max_rect, Rect(top.y, max_left, bottom.y, min_right));
			if(bottom.x > top.x) {
				min_right = std::min(min_right, bottom.x);
			} else {
				max_left = std::max(max_left, bottom.x);
			}
		}
		update(max_rect, Rect(top.y, max_left, 0, min_right));
	}

	for(size_t i=0; i<N; ++i) {
		Point bottom = points[i];
		Num max_left = 0, min_right = X;
		for(size_t j=0; j<i; ++j) {
			Num cur_x = points[j].x;
			if(cur_x > bottom.x) {
				min_right = std::min(min_right, cur_x);
			} else {
				max_left = std::max(max_left, cur_x);
			}
		}
		update(max_rect, Rect(Y, max_left, bottom.y, min_right));
	}

	std::cout << max_rect << " " << N*N << std::endl;

	return EXIT_SUCCESS;
}
