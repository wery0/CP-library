class MO {

    size_t N;

    struct query {
        int l, r, n; int64_t sv;
        //Add needed params
    };

    vector<query> qarr;

    uint64_t hilbertorder(uint64_t x, uint64_t y) {
        const uint64_t logn = __lg(max(x, y) * 2 + 1) | 1;
        const uint64_t maxn = (1ull << logn) - 1;
        uint64_t res = 0;
        for (uint64_t s = 1ull << (logn - 1); s; s >>= 1) {
            bool rx = x & s, ry = y & s;
            res = (res << 2) | (rx ? ry ? 2 : 1 : ry ? 3 : 0);
            if (!rx) {
                if (ry) x ^= maxn, y ^= maxn;
                swap(x, y);
            }
        }
        return res;
    }

public:
    MO() = default;
    MO(size_t N): N(N) {}

    void clear() {qarr.clear();}
    void reserve(size_t Q) {qarr.reserve(Q);}

    void add_query(size_t l, size_t r) {
        assert(l <= r && r < N);
        qarr.emplace_back(l, r, qarr.size());
    }

    template<typename T>
    vector<T> process_queries() {
        for (auto& q : qarr) {
            q.sv = hilbertorder(q.l, q.r);
            //int bl = q.l / BLOCK_SIZE; q.sv = int64_t(bl) * N * 2 + (bl & 1 ? -q.r : q.r);
        }
        sort(qarr.begin(), qarr.end(), [&](const query& q1, const query& q2) {return q1.sv < q2.sv;});
        auto addL = [&](int idx) -> void {

        };
        auto addR = [&](int idx) -> void {

        };
        auto remL = [&](int idx) -> void {

        };
        auto remR = [&](int idx) -> void {

        };
        vector<T> ans(qarr.size());
        for (int cl = 0, cr = -1; const auto& [ql, qr, qn, sv] : qarr) {
            while (cr < qr) addR(++cr);
            while (cl > ql) addL(--cl);
            while (cr > qr) remR(cr--);
            while (cl < ql) remL(cl++);
            ans[qn] = ?;
        }
        return ans;
    }
};
