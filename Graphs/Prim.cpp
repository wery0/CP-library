//Returns vector of edges in mst assuming it exists.
//Complexity: <O(Elog(V)), O(V)>
template<typename T>
vector<pair<int, int>> prim_mst_heap(const vector<vector<pair<int, T>>>& g) {
	const size_t n = g.size();
	if (!n) return {};
	auto chmin = [](auto & x, auto y) {return y < x ? x = y, 1 : 0;};
	vector<pair<int, int>> res(n - 1);
	vector<int> min_edge(n);
	vector<char> in_mst(n);
	vector<T> bw(n, numeric_limits<T>::max());
	if (bool use_std_priority_queue = false; use_std_priority_queue) {
		priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
		pq.emplace(0, 0);
		for (int i = 0; i < n; ++i) {
			auto [bd, v] = pq.top(); pq.pop();
			if (in_mst[v]) {--i; continue;}
			in_mst[v] = 1;
			if (i) res[i - 1] = {v, min_edge[v]};
			for (auto [h, d] : g[v]) {
				if (!in_mst[h] && chmin(bw[h], d)) {
					min_edge[h] = v;
					pq.emplace(d, h);
				}
			}
		}
	} else {
		auto cmp = [&](int l, int r) {return bw[l] < bw[r];};
		vector<int> pq(n + 1, -1), gd(n, -1); int N = 0;
		auto swp = [&](int x, int y) {swap(pq[x], pq[y]); swap(gd[pq[x]], gd[pq[y]]);};
		auto sift_up = [&](int v) {for (; v > 1 && cmp(pq[v], pq[v / 2]); v >>= 1) swp(v, v / 2);};
		auto sift_down = [&](int v) {
			while (v * 2 <= N) {
				const size_t left = v << 1;
				const size_t right = v << 1 | 1;
				size_t swap_with = left;
				if (right <= N && cmp(pq[right], pq[left])) swap_with = right;
				if (cmp(pq[v], pq[swap_with])) break;
				swp(v, swap_with);
				v = swap_with;
			}
		};
		auto push = [&](int x) {
			pq[++N] = x;
			gd[x] = N;
			sift_up(N);
		};
		auto pop = [&]() {
			swp(1, N--);
			gd[pq[N + 1]] = -1;
			sift_down(1);
		};
		push(0);
		for (int i = 0; i < n; ++i) {
			int v = pq[1]; pop();
			in_mst[v] = 1;
			if (i) res[i - 1] = {v, min_edge[v]};
			for (auto [h, d] : g[v]) {
				if (!in_mst[h] && chmin(bw[h], d)) {
					min_edge[h] = v;
					if (gd[h] == -1) {
						push(h);
					} else {
						sift_up(gd[h]);
					}
				}
			}
		}
	}
	return res;
}

//Returns vector of edges in mst assuming it exists.
//Complexity: <O(V ^ 2), O(V)>
template<typename T>
vector<pair<int, int>> prim_mst_quadratic(const vector<vector<T>>& g) {
	const size_t n = g.size();
	if (!n) return {};
	auto chmin = [](auto& x, auto y) {return y < x ? x = y, 1 : 0;};
	vector<pair<int, int>> res(n - 1);
	vector<int> min_edge(n), rem(n);
	vector<T> bw(n, numeric_limits<T>::max());
	iota(rem.begin(), rem.end(), 0);
	for (size_t i = 0, p = 0, v; i < n; ++i) {
		v = rem[p];
		if (i) res[i - 1] = {v, min_edge[v]};
		T mnw = numeric_limits<T>::max();
		swap(rem[p], rem.back()); rem.pop_back();
		for (int j = 0; int h : rem) {
			if (chmin(bw[h], g[v][h])) min_edge[h] = v;
			if (chmin(mnw, bw[h])) p = j;
			++j;
		}
	}
	return res;
}

//Returns vector of edges in mst assuming it exists. f(x, y) must return weight of edge (x, y).
//Complexity: <O(V ^ 2), O(V)>
template<typename T>
vector<pair<int, int>> prim_mst_quadratic_implicit(const size_t n, auto f) {
	if (!n) return {};
	auto chmin = [](auto& x, auto y) {return y < x ? x = y, 1 : 0;};
	vector<pair<int, int>> res(n - 1);
	vector<int> min_edge(n), rem(n);
	vector<T> bw(n, numeric_limits<T>::max());
	iota(rem.begin(), rem.end(), 0);
	for (size_t i = 0, p = 0, v; i < n; ++i) {
		v = rem[p];
		if (i) res[i - 1] = {v, min_edge[v]};
		T mnw = numeric_limits<T>::max();
		swap(rem[p], rem.back()); rem.pop_back();
		for (int j = 0; int h : rem) {
			if (chmin(bw[h], f(h, v))) min_edge[h] = v;
			if (chmin(mnw, bw[h])) p = j;
			++j;
		}
	}
	return res;
}
