class offline_segment_mex {

    struct segtree_point_upd {

        size_t n, U;
        vector<int> mn;

        segtree_point_upd(size_t n): n(n), U(n & (n - 1) ? 2 << __lg(n) : n), mn(U * 2, -1) {}

        int first_less(int x) const {
            assert(mn[1] < x);
            int v = 1;
            for (; v < U;) {
                v <<= 1;
                v |= mn[v] >= x;
            }
            return v - U;
        }

        void point_change(size_t pos, int val) {
            pos += U;
            mn[pos] = val;
            for (pos >>= 1; pos; pos >>= 1) {
                mn[pos] = min(mn[pos << 1], mn[pos << 1 | 1]);
            }
        }
    };

    struct query {size_t l, r, n;};

    vector<query> queries;

public:
    offline_segment_mex(size_t Q = 1) {
        queries.reserve(Q);
    }

    void clear() {queries.clear();}

    void add_query(size_t l, size_t r) {
        assert(l <= r);
        queries.emplace_back(l, r, queries.size());
    }

    //For every query [l, r] returns mex on segment [l, r]
    //Complexity: <O(qlog(q) + nlog(n)), O(n)>
    template<typename T>
    vector<int> process_queries(const vector<T>& m) {
        const size_t N = m.size();
        const size_t Q = queries.size();
        sort(queries.begin(), queries.end(), [](const query& l, const query& r) {return l.r < r.r;});
        vector<int> ans(Q);
        segtree_point_upd lst(N + 1);
        for (size_t i = 0, j = 0; i < Q; ++i) {
            auto [l, r, n] = queries[i];
            assert(r < N);
            while (j <= r) {
                assert(0 <= m[j]);
                if (m[j] <= N) lst.point_change(m[j], j);
                ++j;
            }
            ans[n] = lst.first_less(l);
        }
        return ans;
    }
};
