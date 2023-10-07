class MO {

    int N;
    int BLOCK_SIZE;

    struct query {
        int l, r, n;
        //Add needed params
    };

    vector<query> qarr;

public:
    MO() = default;

    MO(int N, int Q = 1): N(N) {
        BLOCK_SIZE = sqrt(N);
        qarr.reserve(Q);
    }

    void clear() {
        qarr.clear();
    }

    void add_query(int l, int r) {
        assert(0 <= l && l <= r && r < N);
        qarr.emplace_back(l, r, qarr.size());
    }

    vector<int> process_queries() {
        sort(qarr.begin(), qarr.end(), [&](const query& q1, const query& q2) {
            int bl1 = q1.l / BLOCK_SIZE;
            int bl2 = q2.l / BLOCK_SIZE;
            if (bl1 != bl2) return bl1 < bl2;
            return (bool)((q1.r < q2.r) ^ (bl1 & 1));
        });
        auto addL = [&](int i) -> void {

        };
        auto addR = [&](int i) -> void {

        };
        auto remL = [&](int i) -> void {

        };
        auto remR = [&](int i) -> void {

        };
        int cl = 0, cr = -1;
        vector<int> ans(qarr.size());
        for (const auto& [ql, qr, qn] : qarr) {
            while (cr < qr) addR(++cr);
            while (cl > ql) addL(--cl);
            while (cr > qr) remR(cr--);
            while (cl < ql) remL(cl++);
            //Edit
            ans[qn] = ?;
        }
        return ans;
    }
};
