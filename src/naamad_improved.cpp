#include "common.h"

const Num INF = 1e9;

struct Node {
	Point top;
	Num min_left, max_right;

	Node() : top(0,0), min_left(INF), max_right(-1) {}
	void operator=(const Point &p) {
		top = p;
		min_left = p.x;
		max_right = p.x;
	}
};

void update_node(std::vector<Node> &tree, size_t idx) {
	if(tree[idx<<1].min_left == INF) {
		tree[idx] = tree[(idx<<1)+1];
		return;
	}
	if(tree[(idx<<1)+1].max_right == -1) {
		tree[idx] = tree[idx<<1];
		return;
	}
	tree[idx].min_left = tree[idx<<1].min_left;
	tree[idx].max_right = tree[(idx<<1)+1].max_right;
	tree[idx].top = tree[idx<<1].top;
	if(tree[(idx<<1)+1].top.y > tree[idx].top.y) {
		tree[idx].top = tree[(idx<<1)+1].top;
	}
}

void build_tree(size_t N, const std::vector<Point> &points,
		size_t &offset, std::vector<Node> &tree) {
	offset = 1;
	while(offset<N) {
		offset <<= 1;
	}
	tree.resize(offset << 1);
	for(size_t i=0; i<N; ++i) {
		tree[offset+i] = points[i];
	}
	for(size_t i=offset-1; i>=1; --i) {
		update_node(tree, i);
	}
}

Point find_top(const std::vector<Node> &tree, size_t idx, Num L, Num R) {
	if(tree[idx].max_right <= L || R <= tree[idx].min_left) {
		return Point();
	}
	if(L < tree[idx].min_left && tree[idx].max_right < R) {
		return tree[idx].top;
	}
	Point top_L, top_R;
	if(L <= tree[idx<<1].max_right) {
		top_L = find_top(tree, idx<<1, L, R);
	}
	if(tree[(idx<<1)+1].min_left <= R) {
		top_R = find_top(tree, (idx<<1)+1, L, R);
	}
	if(top_R.y > top_L.y) {
		return top_R;
	} else {
		return top_L;
	}
}

void delete_point(std::vector<Node> &tree, size_t idx, size_t offset, const Point &point) {
	if(idx >= offset) {
		tree[idx] = Node();
		return;
	}
	if(tree[idx<<1].top == point) {
		delete_point(tree, idx<<1, offset, point);
	} else if(tree[(idx<<1)+1].top == point) {
		delete_point(tree, (idx<<1)+1, offset, point);
	}
	update_node(tree, idx);
}

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

	size_t offset=0;
	std::vector<Node> tree;
	build_tree(N, points, offset, tree);

	std::vector<size_t> pyx(N);
	for(size_t i=0; i<N; ++i) {
		pyx[i] = i;
	}
	auto descending_Y_order = [&points](const size_t &idx0, const size_t &idx1){
		const Point &p0 = points[idx0];
		const Point &p1 = points[idx1];
		return p0.y == p1.y ? p0.x < p1.x : p0.y > p1.y;
	};
	std::sort(pyx.begin(), pyx.end(), descending_Y_order);

	size_t rr_cnt = 0;
	for(size_t i=0; i<N; ++i) {
		Point top = points[pyx[i]];
		delete_point(tree, 1, offset, top);
		Num max_left = 0, min_right = X;
		while(true) {
			++rr_cnt;
			Point bottom = find_top(tree, 1, max_left, min_right);
			if(bottom.x == 0 && bottom.y == 0) { // no more points
				break;
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

	std::vector<size_t> prev(N), next(N);
	for(int i=0; i<N; ++i) {
		prev[i]=i-1;
		next[i]=i+1;
	}

	for(int i=N-1; i>=0; --i) {
		Point bottom = points[pyx[i]];
		int prev_idx = prev[pyx[i]];
		int next_idx = next[pyx[i]];
		Num max_left=0, min_right=X;
		if(prev_idx != -1) {
			max_left = points[prev_idx].x;
			next[prev_idx]=next_idx;
		}
		if(next_idx != N) {
			min_right = points[next_idx].x;
			prev[next_idx]=prev_idx;
		}
		update(max_rect, Rect(Y, max_left, bottom.y, min_right));
	}

	std::cout << max_rect << " " << rr_cnt << std::endl;

	return EXIT_SUCCESS;
}
