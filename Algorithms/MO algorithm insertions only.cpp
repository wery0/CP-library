//Works when we can do an 'insert' operation, but not a 'remove'.
//For example, when we maintain a minimum difference of numbers in a set.
class MO_insertions {

    size_t N;
    size_t BLOCK_SIZE;

    struct query {
        int l, r, n;
        //Add needed params
    };

    vector<query> qarr;

public:
    MO_insertions() = default;
    MO_insertions(size_t N): N(N), BLOCK_SIZE(sqrt(N)) {}

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
            return bl1 != bl2 ? bl1 < bl2 : q1.r < q2.r;
        });
        T qans = 0;
        auto addL = [&](int idx) -> void {

        };
        auto addR = [&](int idx) -> void {

        };
        auto remL = [&](int idx) -> void {

        };
        vector<T> ans(qarr.size());
        for (int i = 0; i < qarr.size();) {
            int rg = i;
            while (rg + 1 < qarr.size() && qarr[rg + 1].l / BLOCK_SIZE == qarr[i].l / BLOCK_SIZE) ++rg;
            int br = (qarr[i].l / BLOCK_SIZE + 1) * BLOCK_SIZE - 1, rr = br;
            for (; i <= rg; ++i) {
                const auto& [ql, qr, qn] = qarr[i];
                if (qr <= br) {
                    for (int i = ql; i <= qr; ++i) addR(i);
                    ans[qn] = qans;
                    for (int i = ql; i <= qr; ++i) remL(i);
                } else {
                    while (rr < qr) addR(++rr);
                    for (int i = br; i >= ql;) addL(i--);
                    ans[qn] = qans;
                    for (int i = ql; i <= br; ) remL(i++);
                }
            }
            while (br < rr) remL(++br);
        }
        return ans;
    }
};
