#include "common.h"

int main(int argc, char* argv[]) {
	Rect rect;
	std::cin >> rect;
	std::cout << std::setprecision(Num_l::max_digits10) << rect.area << std::endl;
	return EXIT_SUCCESS;
}
