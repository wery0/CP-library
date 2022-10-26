namespace {
    template<typename T>
    void kara(int a, const T *__restrict A, const T *__restrict B, T *__restrict res){
        if(a<=64){
            for(int q=0; q<a; q++){
                for(int w=0; w<a; w++){
                    res[q+w] += A[q]*B[w];
                }
            }
            return;
        }
        const int md = a>>1;
        alignas(64) T sma[a], E[a];
        init(E, 0);
        const auto smb = sma+md;
        for(int q=0; q<md; q++){
            sma[q] = A[q]+A[q+md];
            smb[q] = B[q]+B[q+md];
        }
        kara(md, sma, smb, E);
        kara(md, A+0, B+0, res);
        kara(md, A+md, B+md, res+a);
        for(int q=0; q<md; q++){
            const auto lol = res[q+md];
            res[q+md] += E[q] - res[q] - res[q+md*2];
            res[q+md*2] += E[q+md] - lol - res[q+md*3];
        }
    }
}

namespace {
    template<int a, typename T>
    void kara(const T *__restrict A, const T *__restrict B, T* __restrict res) {
        if (a <= 32) {
            for (int q = 0; q < a; q++) {
                for (int w = 0; w < a; w++) {
                    res[q + w] += A[q] * B[w];
                }
            }
            return;
        }
        const int md = a >> 1;
        T E[a] = {}, sma[a];
        auto smb = sma + md;
        for (int q = 0; q < md; q++) {
            sma[q] = A[q] + A[q + md];
            smb[q] = B[q] + B[q + md];
        }
        kara<md>(sma, smb, E);
        kara<md>(A, B, res);
        kara<md>(A + md, B + md, res + md * 2);
        for (int q = 0; q < md; q++) {
            const auto kek = res[q + md];
            res[q + md] += E[q] - res[q] - res[q + md * 2];
            res[q + md * 2] += E[q + md] - kek - res[q + md * 3];
        }
    }
}