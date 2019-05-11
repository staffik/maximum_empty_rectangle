#include "common.h"
#include <set>
//#include <cassert>

const Num INF = 1e15;

bool contain(Num x, Num l, Num r) {
	return l <= x && x <= r;
}

bool strict_intersect(Num l0, Num r0, Num l1, Num r1) {
	return !(r0 <= l1 || r1 <= l0);
}

struct Boundary {
	char is_top;
	char is_flat;
	Num l, r, h;

	char mode() const {
		if(is_top && is_flat) {
			return 'a';
		}
		return is_top ? 'c' : 'b';
	}

	Boundary() = default;
	Boundary(char is_top_, char is_flat_, Num l_, Num r_, Num h_) {
		is_top = is_top_; is_flat = is_flat_; l = l_; r = r_; h = h_;
	}
	Boundary(char is_top_, const Rect &rect) {
		is_top = is_top_;
		is_flat = (rect.t == rect.b);
		l = rect.l;
		r = rect.r;
		h = is_top ? rect.t : rect.b;
	}
};

struct Window {
	Num l, r, h;
	Window(Num l, Num r, Num h) : l(l), r(r), h(h) {}
};

struct Node {
	Num max_left, max_right;
	std::vector<Window> L[3];
	size_t active_l, active_r;

	Node() : max_left(INF), max_right(INF),
		active_l(0), active_r(0) {}

	bool empty_lists() {
		for(size_t side=0; side<3; ++side) {
			if(!L[side].empty()) {
				return false;
			}
		}
		return true;
	}

	bool is_active() {
		
		return !empty_lists() || (active_l && active_r);
	}

	size_t get_active_son() {
		return std::max(active_l, active_r);
	}
};

void update_node(std::vector<Node> &tree, size_t idx) {
	size_t al = tree[idx].active_l;
	size_t ar = tree[idx].active_r;
	if(al && !tree[al].is_active()) {
		tree[idx].active_l = tree[al].get_active_son();
	}
	if(ar && !tree[ar].is_active()) {
		tree[idx].active_r = tree[ar].get_active_son();
	}
}

void build_tree(size_t N, Num X, std::vector<Rect> &rects,
		size_t &offset, std::vector<Node> &tree) {
	std::vector<Num> x_coords(N<<1);
	for(size_t i=0; i<N; ++i) {
		x_coords[i<<1]=rects[i].l;
		x_coords[(i<<1)+1]=rects[i].r;
	}
	x_coords.push_back(0);
	x_coords.push_back(X);
	std::sort(x_coords.begin(), x_coords.end());
	auto it = std::unique(x_coords.begin(), x_coords.end());
	size_t n = std::distance(x_coords.begin(), it);
	x_coords.resize(n);
	
	offset = 1;
	while(offset<n) {
		offset <<= 1;
	}
	tree.resize(offset << 1);
	for(size_t i=0; i<n; ++i) {
		tree[offset+i].max_left = tree[offset+i].max_right = x_coords[i];
	}
	for(size_t i=offset-1; i>=1; --i) {
		tree[i].max_left = tree[i<<1].max_right;
		tree[i].max_right = tree[(i<<1)+1].max_right;
		if(tree[i].max_right == INF) {
			tree[i].max_right = tree[i].max_left;
		}
	}
}

size_t find_node(std::vector<Node> &tree, size_t W, Num l, Num r) {
	while(!contain(tree[W].max_left, l, r)) {
		if(r < tree[W].max_left) {
			W <<= 1;
		} else {
			W = (W<<1) + 1;
		}
	}
	return W;
}

size_t find_leaf(std::vector<Node> &tree, size_t offset, Num x) {
	size_t W = 1;
	while(W<offset) {
		if(x <= tree[W].max_left) {
			W <<= 1;
		} else {
			W = (W<<1) + 1;
		}
	}
	return W;
}
	
void insert_window(std::vector<Node> &tree, size_t W, Window window) {
	int mode=-1;
	if(window.r == tree[W].max_left) {
		mode = 0;
	} else if(window.l == tree[W].max_left) {
		mode = 2;
	} else {
		mode = 1;
	}
	/*
	if(mode == 1) {
		//assert(tree[W].L[0].empty() || window.l <= tree[W].L[0].back().l);
		//assert(tree[W].L[2].empty() || window.r >= tree[W].L[2].back().r);
	} else {
		//assert(tree[W].L[1].empty());
	}
	*/
	if(tree[W].L[mode].empty()) {
		tree[W].L[mode].push_back(window);
	} else {
		Window w = tree[W].L[mode].back();
		//assert(window.l <= w.l && w.r <= window.r);
		if(window.l < w.l || w.r < window.r) {
			tree[W].L[mode].push_back(window);
		}
	}
}

void remove_intersecting_from_node(Node &node, std::vector<Window> &windows, Num l, Num r) {
	for(size_t side = 0; side < 3; ++side) {
		while(!node.L[side].empty()) {
			Window window = node.L[side].back();
			if(!strict_intersect(window.l, window.r, l, r)) {
			       break;
			}
			windows.push_back(window);
			node.L[side].pop_back();
		}
	}
}

void remove_intersecting_from_subtree(std::vector<Node> &tree, size_t idx,
				std::vector<Window> &windows, Num l, Num r) {
	remove_intersecting_from_node(tree[idx], windows, l, r);
	size_t al = tree[idx].active_l;
	size_t ar = tree[idx].active_r;
	if(al) {
		remove_intersecting_from_subtree(tree, al, windows, l, r);
		//assert(tree[al].empty_lists());
		tree[idx].active_l = 0;
	}
	if(ar) {
		remove_intersecting_from_subtree(tree, ar, windows, l, r);
		//assert(tree[ar].empty_lists());
		tree[idx].active_r = 0;
	}
}

void remove_intersecting(std::vector<Window> &windows, std::vector<Node> &tree,
			size_t offset, Num l, Num r) {
	size_t W = find_node(tree, 1, l, r);
	size_t L = find_leaf(tree, offset, l);
	size_t R = find_leaf(tree, offset, r);
	while(L != W) {
		remove_intersecting_from_node(tree[L], windows, l, r);
		remove_intersecting_from_node(tree[R], windows, l, r);
		if(L != R) {
			size_t ar = tree[L].active_r;
			if(ar) {
				remove_intersecting_from_subtree(tree, ar, windows, l, r);
				//assert(tree[ar].empty_lists());
				tree[L].active_r = 0;
			}
			size_t al = tree[R].active_l;
			if(al) {
				remove_intersecting_from_subtree(tree, al, windows, l, r);
				//assert(tree[al].empty_lists());
				tree[R].active_l = 0;
			}
		}
		L >>= 1;
		R >>= 1;
		update_node(tree, L);
		update_node(tree, R);
	}
	while(W > 0) {
		remove_intersecting_from_node(tree[W], windows, l, r);
		W >>= 1;
		update_node(tree, W);
	}
}

void insert_windows(std::vector<Node> &tree, size_t idx, std::vector<Window> &windows) {
	for(int i = (int)windows.size()-1; i>=0; --i) {
		insert_window(tree, idx, windows[i]);
	}
}

void split_window_and_insert(Window w, std::vector<Node> &tree, size_t &ptr_l, size_t &ptr_r,
		std::vector<Window> &buffer_left, std::vector<Window> &buffer_right, Num l, Num r) {
	if(w.l < l) {
		Window left_window(w.l, l, w.h);
		size_t new_ptr_l = find_node(tree, ptr_l, left_window.l, left_window.r);
		if(new_ptr_l != ptr_l) {
			insert_windows(tree, ptr_l, buffer_left);
			buffer_left.clear();
			ptr_l = new_ptr_l;
		}
		buffer_left.push_back(left_window);
	}
	if(w.r > r) {
		Window right_window(r, w.r, w.h);
		size_t new_ptr_r = find_node(tree, ptr_r, right_window.l, right_window.r);
		if(new_ptr_r != ptr_r) {
			insert_windows(tree, ptr_r, buffer_right);
			buffer_right.clear();
			ptr_r = new_ptr_r;
		}
		buffer_right.push_back(right_window);
	}
}

void update_up(std::vector<Node> &tree, size_t W) {
	//assert(W==1 || tree[W].is_active());
	size_t last_active = W;
	while(W>1) {
		if(W%2) {
			tree[W>>1].active_r = last_active;
		} else {
			tree[W>>1].active_l = last_active;
		}
		W >>= 1;
		if(tree[W].is_active()) {
			last_active = W;
		}
	}
}

int main(int argc, char* argv[]) {
	Num X, Y;
	size_t N;
	std::vector<Rect> rects;
	read_input(std::cin, X, Y, N, rects);

	size_t offset=0;
	std::vector<Node> tree;
	build_tree(N, X, rects, offset, tree);

	std::vector<Boundary> boundaries(2*N);
	for(size_t i=0; i<N; ++i) {
		boundaries[i<<1] = Boundary(true, rects[i]);
		boundaries[(i<<1)+1] = Boundary(false, rects[i]);
	}
	boundaries.push_back(Boundary(0, 1, 0, X, Y));
	boundaries.push_back(Boundary(1, 1, 0, X, 0));
	auto boundaries_cmp = [](const Boundary &b0, const Boundary &b1) {
		if(b0.h != b1.h) {
			return b0.h > b1.h;
		}
		if(b0.mode() != b1.mode()) {
			return b0.mode() < b1.mode();
		}
		return b0.l < b1.l;
	};
	std::sort(boundaries.begin(), boundaries.end(), boundaries_cmp);

	std::set<Num> left_ends, right_ends;
	left_ends.insert(X);
	right_ends.insert(0);
	
	Rect max_rect;
	size_t rr_cnt=0;
	for(auto& boundary : boundaries) {
		if(boundary.is_top) {
			std::vector<Window> windows;
			remove_intersecting(windows, tree, offset, boundary.l, boundary.r);
			auto window_cmp = [](const Window &w0, const Window &w1) {
				return w0.l != w1.l ? w0.l < w1.l : w0.r > w1.r;
			};
			std::sort(windows.begin(), windows.end(), window_cmp);
			size_t ptr_l=1, ptr_r=1;
			std::vector<Window> buffer_left, buffer_right;
			for(auto& w : windows) {
				update(max_rect, Rect(w.h, w.l, boundary.h, w.r));
				++rr_cnt;
				split_window_and_insert(w, tree, ptr_l, ptr_r,
						buffer_left, buffer_right, boundary.l, boundary.r);
			}
			if(!buffer_left.empty()) {
				insert_windows(tree, ptr_l, buffer_left);
			}
			if(!buffer_right.empty()) {
				insert_windows(tree, ptr_r, buffer_right);
			}

			update_up(tree, ptr_l);
			update_up(tree, ptr_r);
			
			if(!boundary.is_flat) {
				left_ends.insert(boundary.r);
				right_ends.insert(boundary.l);
			}
		} else {
			auto l = right_ends.upper_bound(boundary.l);
			--l;
			auto r = left_ends.lower_bound(boundary.r);
			size_t W = find_node(tree, 1, *l, *r);
			insert_window(tree, W, Window(*l, *r, boundary.h));
			update_up(tree, W);
			if(!boundary.is_flat) {
				left_ends.erase(boundary.l);
				right_ends.erase(boundary.r);
			}
		}
	}

	std::cout << max_rect << " " << rr_cnt << std::endl;

	return EXIT_SUCCESS;
}
