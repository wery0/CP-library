class MO {

    size_t N;
    size_t BLOCK_SIZE;

    struct query {
        int l, r, n;
        //Add needed params
    };

    vector<query> qarr;

public:
    MO() = default;
    MO(size_t N): N(N), BLOCK_SIZE(sqrt(N)) {}

    void clear() {qarr.clear();}
    void reserve(size_t Q) {qarr.reserve(Q);}

    void add_query(size_t l, size_t r) {
        assert(l <= r && r < N);
        qarr.emplace_back(l, r, qarr.size());
    }

    template<typename T>
    vector<T> process_queries() {
        sort(qarr.begin(), qarr.end(), [&](const query& q1, const query& q2) {
            int bl1 = q1.l / BLOCK_SIZE;
            int bl2 = q2.l / BLOCK_SIZE;
            if (bl1 != bl2) return bl1 < bl2;
            return bl1 & 1 ? q1.r > q2.r : q1.r < q2.r;
        });
        auto addL = [&](int idx) -> void {

        };
        auto addR = [&](int idx) -> void {

        };
        auto remL = [&](int idx) -> void {

        };
        auto remR = [&](int idx) -> void {

        };
        int cl = 0, cr = -1;
        vector<T> ans(qarr.size());
        for (const auto& [ql, qr, qn] : qarr) {
            while (cr < qr) addR(++cr);
            while (cl > ql) addL(--cl);
            while (cr > qr) remR(cr--);
            while (cl < ql) remL(cl++);
            ans[qn] = ?;
        }
        return ans;
    }
};
