//Works when we can do an 'insert' operation, but not a 'remove'.
//For example, when we maintain a minimum difference of numbers in a set.
namespace MO {

    //Edit, if need
    const int MAXN = 300000;
    const int MAXQ = 300000;
    const int BLOCK_SIZE = (int)sqrt(MAXN);

    struct query {
        int l, r, n;
        //Add needed params
    };

    query qarr[MAXQ];
    int qsz = 0;
    int ans[MAXQ];

    inline void add_query(int l, int r) {
        qarr[qsz]  = {l, r, qsz};
        ++qsz;
    }

    inline void addL(int i) {
    }

    inline void addR(int i) {
    }

    inline void remL(int i) {
    }

    void go() {
        sort(qarr, qarr + qsz, [](const query& q1, const query& q2) {
            int bl1 = q1.l / BLOCK_SIZE;
            int bl2 = q2.l / BLOCK_SIZE;
            if (bl1 != bl2) return bl1 < bl2;
            return q1.r < q2.r;
        });
        for (int i = 0; i < qsz;) {
            int rg = i;
            while (rg + 1 < qsz && qarr[rg + 1].l / BLOCK_SIZE == qarr[i].l / BLOCK_SIZE) ++rg;
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
                    for (; rr < qr; ) addR(++rr);
                    int was = ansr;
                    for (int i = br; i >= ql;) addL(i--);
                    ans[qn] = ansr;

                    ansr = was;
                    for (int i = ql; i <= br; ) remL(i++);
                }
            }
        }
    }
};