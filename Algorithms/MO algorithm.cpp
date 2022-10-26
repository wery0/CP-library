namespace MO {

    //edit, if need
    const int MAXN = 300000;
    const int MAXQ = 300000;
    const int BLOCK_SIZE = 512;

    struct query {
        int l, r, n;
        //add needed params
    };

    query qarr[MAXQ];
    int qsz = 0;
    ll ans[MAXQ];

    inline void add_query(int l, int r) {
        qarr[qsz]  = {l, r, qsz};
        ++qsz;
    }

    inline void addR(int ps) {

    }

    inline void addL(int ps) {

    }

    inline void remR(int ps) {

    }

    inline void remL(int ps) {

    }

    void go() {
        sort(qarr, qarr + qsz, [](const query & q1, const query & q2) {
            int bl1 = q1.l / BLOCK_SIZE;
            int bl2 = q2.l / BLOCK_SIZE;
            if (bl1 != bl2) return bl1 < bl2;
            return (bool)((q1.r < q2.r) ^ (bl1 & 1));
        });
        int cl = 0, cr = -1;
        for (int q = 0; q < qsz; q++) {
            const auto &i = qarr[q];
            while (cr < i.r) addR(++cr);
            while (cl > i.l) addL(--cl);
            while (cr > i.r) remR(cr--);
            while (cl < i.l) remL(cl++);
            //edit
            ans[i.n] = 998244353;
        }
    }
};