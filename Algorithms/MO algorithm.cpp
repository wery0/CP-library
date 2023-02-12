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
        qarr[qsz] = {l, r, qsz};
        ++qsz;
    }

    inline void addR(int i) {
    }

    inline void addL(int i) {
    }

    inline void remR(int i) {
    }

    inline void remL(int i) {
    }

    void go() {
        sort(qarr, qarr + qsz, [](const query& q1, const query& q2) {
            int bl1 = q1.l / BLOCK_SIZE;
            int bl2 = q2.l / BLOCK_SIZE;
            if (bl1 != bl2) return bl1 < bl2;
            return (bool)((q1.r < q2.r) ^ (bl1 & 1));
        });
        int cl = 0, cr = -1;
        for (int i = 0; i < qsz; i++) {
            const auto& [ql, qr, qn] = qarr[i];
            while (cr < qr) addR(++cr);
            while (cl > ql) addL(--cl);
            while (cr > qr) remR(cr--);
            while (cl < ql) remL(cl++);
            //Edit
            ans[qn] = 998244353;
        }
    }
};