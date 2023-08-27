template<typename T>
class offline_segment_max_leq {

    struct segtree_point_upd {

        size_t n;
        vector<T> mx;

        segtree_point_upd(size_t n, T val): n(n), mx(n * 2, val) {
        }

        T seg_max(size_t l, size_t r) const {
            l += n, r += n;
            T ans = mx[r];
            for (; l <= r; l = (l + 1) >> 1, r = (r - 1) >> 1) {
                if (l & 1) ans = max(ans, mx[l]);
                if (~r & 1) ans = max(ans, mx[r]);
            }
            return ans;
        }

        void point_change(size_t pos, T val) {
            pos += n;
            mx[pos] = val;
            for (pos >>= 1; pos; pos >>= 1) {
                mx[pos] = max(mx[pos << 1], mx[pos << 1 | 1]);
            }
        }
    };

    struct query {size_t l, r, n; T x;};

    vector<query> queries;

public:
    offline_segment_max_leq(size_t Q = 1) {
        queries.reserve(Q);
    }

    void clear() {queries.clear();}

    void add_query(size_t l, size_t r, T x) {
        assert(l <= r);
        queries.emplace_back(l, r, queries.size(), x);
    }

    //For every query [l, r, x] returns maximum number on segment [l, r], which <= x
    //If all numbers on segment [l, r] > x, then returns x + 1
    //O(QlogN)
    vector<T> process_queries(const vector<T>& m) {
        const size_t N = m.size();
        const size_t Q = queries.size();
        const T inf = numeric_limits<T>::max();
        sort(queries.begin(), queries.end(), [](const query& l, const query& r) {return l.x < r.x;});
        vector<size_t> u(N);
        iota(u.begin(), u.end(), 0);
        sort(u.begin(), u.end(), [&](const auto& l, const auto& r) {return m[l] < m[r];});
        vector<T> ans(Q);
        segtree_point_upd spu(N, -inf);
        for (size_t i = 0, j = 0; i < Q; ++i) {
            auto [l, r, n, x] = queries[i];
            assert(r < N);
            while (j < N && m[u[j]] <= x) {
                spu.point_change(u[j], m[u[j]]);
                ++j;
            }
            T tyt = spu.seg_max(l, r);
            ans[n] = tyt == -inf ? x + 1 : tyt;
        }
        return ans;
    }
};
