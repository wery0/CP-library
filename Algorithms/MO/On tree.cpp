class MO_on_tree {

    size_t N;
    vector<int> tin, tout, eul, pr, dep, jump;

    struct query {
        int l, r, n, lc; int64_t sv;
        //Add needed params
    };

    vector<query> qarr;

    int calc_kth_ancestor(int v, int k) {
        for (; k;) {
            int u = jump[v], dfd = dep[v] - dep[u];
            if (dfd <= k) v = u, k -= dfd;
            else v = pr[v], k--;
        }
        return v;
    }

    int calc_lca(int x, int y) {
        if (dep[x] > dep[y]) swap(x, y);
        y = calc_kth_ancestor(y, dep[y] - dep[x]);
        if (x == y) return x;
        for (; pr[x] != pr[y];) {
            int u1 = jump[x], u2 = jump[y];
            if (u1 == u2) x = pr[x], y = pr[y];
            else x = u1, y = u2;
        }
        return pr[x];
    }

    template<typename T>
    T hilbert_encode(T x, T y) {
        static_assert(is_unsigned_v<T>);
        const T logn = __lg(max(x, y) * 2 + 1) | 1;
        const T maxn = (T(1) << logn) - 1;
        T p = 0;
        for (T s = T(1) << (logn - 1); s; s >>= 1) {
            bool rx = x & s, ry = y & s;
            p = (p << 2) | (rx ? ry ? 2 : 1 : ry ? 3 : 0);
            if (!rx) {
                if (ry) x ^= maxn, y ^= maxn;
                swap(x, y);
            }
        }
        return p;
    }

public:
    MO_on_tree() = default;
    MO_on_tree(vector<pair<int, int>> edges): N(edges.size() + 1), tin(N), tout(N), eul(N * 2), pr(N), dep(N), jump(N) {
        vector l(N, vector<int>());
        for (auto [x, y] : edges) {
            assert(0 <= x && x < N);
            assert(0 <= y && y < N);
            l[x].push_back(y);
            l[y].push_back(x);
        }
        function<void(int, int)> dfs = [&](int v, int p) {
            static int T = 0;
            pr[v] = p;
            tin[v] = T; eul[T++] = v;
            dep[v] = (p == -1 ? -1 : dep[p]) + 1;
            if (dep[v] == 0) {
                jump[v] = v;
            } else if (dep[pr[v]] + dep[jump[jump[pr[v]]]] == dep[jump[pr[v]]] * 2) {
                jump[v] = jump[jump[pr[v]]];
            } else {
                jump[v] = pr[v];
            }
            for (int h : l[v]) {
                if (h != p) dfs(h, v);
            }
            tout[v] = T; eul[T++] = v;
        };
        dfs(0, -1);
    }

    void clear() {qarr.clear();}
    void reserve(size_t Q) {qarr.reserve(Q);}

    void add_query(size_t x, size_t y) {
        assert(x < N && y < N);
        if (tin[x] > tin[y]) swap(x, y);
        int lc = calc_lca(x, y);
        if (x == lc) qarr.emplace_back(tin[x], tin[y], qarr.size(), -1);
        else qarr.emplace_back(tout[x], tin[y], qarr.size(), lc);
    }

    template<typename T>
    vector<T> process_queries() {
        const size_t BLOCK_SIZE = min<size_t>(N / sqrt(qarr.size() + 1) + 1, N);
        for (auto& q : qarr) {
            q.sv = hilbert_encode<uint64_t>(q.l, q.r);
            //int bl = q.l / BLOCK_SIZE; q.sv = int64_t(bl) * N * 2 + (bl & 1 ? -q.r : q.r);
        }
        sort(qarr.begin(), qarr.end(), [&](const query& q1, const query& q2) {return q1.sv < q2.sv;});
        vector<char> cnt(N);
        auto add = [&](int idx) -> void {

        };
        auto rem = [&](int idx) -> void {

        };
        auto flip = [&](int idx) -> void {
            ((cnt[eul[idx]] ^= 1) ? add(idx) : rem(idx));
        };
        vector<T> ans(qarr.size());
        for (int cl = 0, cr = -1; const auto& [ql, qr, qn, lc, sv] : qarr) {
            while (cr < qr) flip(++cr);
            while (cl > ql) flip(--cl);
            while (cr > qr) flip(cr--);
            while (cl < ql) flip(cl++);
            if (lc != -1) add(tin[lc]);
            ans[qn] = ?;
            if (lc != -1) rem(tin[lc]);
        }
        return ans;
    }
};
