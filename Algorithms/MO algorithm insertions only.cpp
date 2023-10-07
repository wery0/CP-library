//Works when we can do an 'insert' operation, but not a 'remove'.
//For example, when we maintain a minimum difference of numbers in a set.
class MO_insertions {

    int N;
    int BLOCK_SIZE;

    struct query {
        int l, r, n;
        //Add needed params
    };

    vector<query> qarr;

public:
    MO_insertions() = default;

    MO_insertions(int N, int Q = 1): N(N) {
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
            return q1.r < q2.r;
        });
        auto addL = [&](int i) -> void {

        };
        auto addR = [&](int i) -> void {

        };
        auto remL = [&](int i) -> void {

        };
        vector<int> ans(qarr.size());
        for (int i = 0; i < qarr.size();) {
            int rg = i;
            while (rg + 1 < qarr.size() && qarr[rg + 1].l / BLOCK_SIZE == qarr[i].l / BLOCK_SIZE) ++rg;
            int br = (qarr[i].l / BLOCK_SIZE + 1) * BLOCK_SIZE - 1;

            //Deal with que_ans and ansr.
            for (int rr = br, ansr = 0; i <= rg; ++i) {
                const auto& [ql, qr, qn] = qarr[i];
                if (qr <= br) {            //When the query fully lies inside a single block
                    int que_ans = 0;
                    for (int i = ql; i <= qr; ++i) addR(i);
                    ans[qn] = que_ans;

                    for (int i = qr; i >= ql; --i) remL(i);
                } else {
                    while (rr < qr) addR(++rr);
                    int was = ansr;
                    for (int i = br; i >= ql;) addL(i--);
                    ans[qn] = ansr;

                    ansr = was;
                    for (int i = ql; i <= br; ) remL(i++);
                }
            }
        }
        return ans;
    }
};
