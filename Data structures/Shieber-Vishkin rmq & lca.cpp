/*
Shieber-Vishkin algorithm for rmq in O(1) using O(n) precomputation
  cmp = less()          -> rightest min
  cmp = less_equal()    -> leftest min
  cmp = greater()       -> rightest max
  cmp = greater_equal() -> leftest max
https://codeforces.com/blog/entry/125371
*/
template<typename T, auto cmp = less()>
class shieber_vishkin_rmq {
    int n;
    vector<T> m;
    vector<int> in, asc, head;

public:
    //<O(n), O(n)>
    shieber_vishkin_rmq(const vector<T>& m): n(m.size()), m(m), in(n), asc(n), head(n + 1) {
        vector<int> st = {-1};
        for (int i = 0; i < n + 1; ++i) {
            int prev = 0;
            while (st.size() > 1 && (i == n || !cmp(m[st.back()], m[i]))) {
                head[prev] = st.back();
                auto j = end(st)[-2] + 1u, k = bit_floor(i ^ j);
                in[st.back()] = prev = i & -k, asc[j] |= k;
                st.pop_back();
            }
            st.push_back(head[prev] = i);
        }
        for (int i = 1; i < n; ++i) asc[i] = (asc[i] | asc[i - 1]) & -(in[i] & -in[i]);
    }

    //O(1)
    int rmq_position(size_t l, size_t r) const {
        assert(l <= r && r < n);
        if (uint32_t j = in[l] ^ in[r]) {
            j = asc[l] & asc[r] & -bit_floor(j);
            if (auto k = bit_floor(asc[l] ^ j)) l = head[(in[l] & -k) | k];
            if (auto k = bit_floor(asc[r] ^ j)) r = head[(in[r] & -k) | k];
        }
        return cmp(m[l], m[r]) ? l : r;
    }

    //O(1)
    T rmq(size_t l, size_t r) const {return m[rmq_position(l, r)];}
};

//Shieber-Vishkin algorithm for lca in O(1) using O(n) precomputation
//https://codeforces.com/blog/entry/125371
class shieber_vishkin_lca {
    int n;
    vector<int> dep, in, asc, head;

public:
    //<O(n), O(n)>
    shieber_vishkin_lca(const auto& l, int root): n(l.size()), dep(n), in(n), asc(n), head(n + 1) {
        vector<pair<int, int>> order; order.reserve(n);
        auto lsb = [](int x) {return x & -x;};
        function<void(int, int)> dfs = [&](int v, int p) {
            order.emplace_back(v, p);
            in[v] = order.size();
            for (int h : l[v]) {
                if (h == p) continue;
                dep[h] = dep[v] + 1;
                dfs(h, v);
                head[in[h]] = v;
                if (lsb(in[v]) < lsb(in[h])) in[v] = in[h];
            }
        };
        dfs(root, -1);
        for (auto [v, p] : order) asc[v] = asc[p] | lsb(in[v]);
    }

    //O(1)
    int lca(int x, int y) const {
        if (uint32_t j = in[x] ^ in[y]) {
            j = asc[x] & asc[y] & -bit_floor(j);
            if (auto k = bit_floor(asc[x] ^ j)) x = head[(in[x] & -k) | k];
            if (auto k = bit_floor(asc[y] ^ j)) y = head[(in[y] & -k) | k];
        }
        return dep[x] < dep[y] ? x : y;
    }
};
