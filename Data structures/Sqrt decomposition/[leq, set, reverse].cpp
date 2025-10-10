//https://codeforces.com/group/n8PnRC2wrr/contest/271555/problem/A
//https://ejudge.algocode.ru/cgi-bin/new-client?SID=bde9f1b076e6cedf&action=139&prob_id=1
//https://basecamp.eolymp.com/ru/problems/2961
template<typename T>
class sqrt_seg_set_leq_rev {
    #define duration(a) chrono::duration_cast<chrono::nanoseconds>(a).count()

    struct block {
        T ps;
        uint32_t stm;
    };

    size_t N, nB = 0;
    T* m, *buf;
    uint32_t* sm, *st;
    block* h;
    uint64_t* flex = 0;

    size_t calc_B(double coef) const {return min(N, max<size_t>(1, sqrt(N * __lg(N + 1)) * coef));}
    size_t block_length(size_t i) const {return st[i + 1] - st[i];}
    size_t get_block(size_t i) const {return upper_bound(st, st + nB + 1, i) - st - 1;}

    size_t nq = 0;
    array<size_t, 4> times{0};
    void chk() {
        static constexpr double sl = 1.1, sr = 1.2;
        static double c1 = 1, c2 = 4, oc1 = c1, oc2 = c2, s1 = sl, s2 = sl;
        static size_t onq = 0, otq = -1;
        size_t tq = accumulate(times.begin() + 1, times.end(), size_t(0));
        if (tq > times[0] * c2) {
            swap(oc1, c1);
            swap(oc2, c2);
            if (1.0 * nq / (tq + times[0]) >= 1.0 * onq / otq) {
                c1 *= c1 < oc1 ? s1 : 1 / s1;
                c2 *= c2 < oc2 ? s2 : 1 / s2;
            }
            onq = nq, otq = tq + times[0];
            c1 = max(c1, 0.1), c2 = max(c2, 0.1);
            s1 *= 0.998, s2 *= 0.997;
            if (s1 < sl) s1 += sr - sl;
            if (s2 < sl) s2 += sr - sl;
            size_t nwB = calc_B(c1);
            rebuild(nwB);
            cerr << c1 << ' ' << c2 << " -> " << nwB << endl;
            fill(times.begin() + 1, times.end(), 0);
            nq = 0;
        }
    }

    size_t seg_leq(const size_t l, const size_t r, T x) const {
        assert(l <= r && r < N);
        auto smart = [&](size_t b) -> size_t {
            if (h[b].ps & 1) return (h[b].ps >> 2) <= x ? block_length(b) : 0;

            // int L = h[b].stm, R = L + block_length(b);
            // L -= 1;
            // while (L + 1 < R) {
            //     int md = L + (R - L) / 2;
            //     (m[sm[md]] <= x ? L : R) = md;
            // }
            // return L + 1- h[b].stm;

            // int L = h[b].stm, D = block_length(b) + 1;
            // L -= 1;
            // while (D > 1) {
            //     int h = D / 2;
            //     L += (m[sm[L + h]] <= x) * h;
            //     D -= h;
            // }
            // return L + 1- h[b].stm;
 
            return upper_bound(sm + h[b].stm, sm + h[b].stm + block_length(b), x, [&](T x, int l) {return x < m[l];}) - sm - h[b].stm;
        };
        auto stupid = [&](const size_t l, const size_t r, const size_t b) -> size_t {
            if (h[b].ps & 1) return (h[b].ps >> 2) <= x ? r - l + 1 : 0;
            size_t res = 0;
            const size_t t = h[b].stm + (h[b].ps & 2 ? st[b + 1] - 1 - r : l - st[b]);
            for (size_t i = 0; i < r - l + 1; ++i) res += m[t + i] <= x;
            return res;
        };
        const size_t bl = get_block(l), br = get_block(r);
        size_t res = 0;
        if (bl == br) {
            res += stupid(l, r, bl);
        } else {
            res += stupid(l, st[bl + 1] - 1, bl);
            res += stupid(st[br], r, br);
            for (size_t i = bl + 1; i < br; ++i) res += smart(i);
        }
        return res;
    }

    void seg_set(const size_t l, const size_t r, T x) {
        assert(l <= r && r < N);
        auto stupid = [&](size_t l, size_t r, const size_t b) {
            const size_t L = h[b].stm, R = L + block_length(b);
            if (h[b].ps & 1) {
                fill(m + L, m + R, h[b].ps >> 2);
                h[b].ps = 0;
            } else if (h[b].ps & 2) {
                l = st[b] + st[b + 1] - 1 - l;
                r = st[b] + st[b + 1] - 1 - r;
                swap(l, r);
            }
            const size_t t = L + l - st[b];
            for (size_t i = 0; i < r - l + 1; ++i) m[t + i] = x;
            l += L - st[b], r += L - st[b];
            size_t s1 = 0, s2 = r - l + 1;
            for (size_t i = L; i < R; ++i) {
                buf[l <= sm[i] && sm[i] <= r ? s1++ : s2++] = sm[i];
            }
            merge(buf, buf + s1, buf + s1, buf + s2, sm + L, [&](int x, int y) {return m[x] < m[y];});
        };
        const size_t bl = get_block(l), br = get_block(r);
        if (bl == br) {
            stupid(l, r, bl);
        } else {
            stupid(l, st[bl + 1] - 1, bl);
            stupid(st[br], r, br);
            for (size_t i = bl + 1; i < br; ++i) {
                h[i].ps = (x << 2) | 1;
            }
        }
    }

    void seg_reverse(const size_t l, const size_t r) {
        assert(l <= r && r < N);
        if (l) split(l - 1);
        split(r);
        const size_t bl = get_block(l), br = get_block(r);
        reverse(h + bl, h + br + 1);
        for (size_t i = bl, u = br - bl + 1; i <= br; ++i) {
            buf[--u] = block_length(i);
            h[i].ps ^= 2;
        }
        for (size_t i = bl; i <= br; ++i) st[i + 1] = st[i] + buf[i - bl];
    }

    void split(const size_t i) {
        const size_t b = get_block(i), L = h[b].stm, R = L + block_length(b);
        if (i == st[b + 1] - 1) return;
        h[nB] = {h[b].ps, 0};
        rotate(h + b + 1, h + nB, h + nB + 1);
        st[++nB] = i + 1;
        rotate(st + b + 1, st + nB, st + nB + 1);
        if (~h[b].ps & 1) {
            if (h[b].ps & 2) {
                h[b + 1].stm = L + block_length(b + 1);
                swap(h[b].stm, h[b + 1].stm);
                size_t s1 = block_length(b + 1), s2 = 0;
                for (size_t i = L; i < R; ++i) {
                    buf[sm[i] >= h[b].stm ? s1++ : s2++] = sm[i];
                }
                copy(buf, buf + s1, sm + L);
            } else {
                h[b + 1].stm = L + block_length(b);
                size_t s1 = 0, s2 = block_length(b);
                for (size_t i = L; i < R; ++i) {
                    buf[sm[i] < h[b + 1].stm ? s1++ : s2++] = sm[i];
                }
                copy(buf, buf + s2, sm + L);
            }
        } else {
            h[b + 1].stm = L + block_length(b);
            iota(sm + L, sm + R, h[b].stm);
        }
    }

    void rebuild(const size_t B) {
        auto stt = timeStamp();
        const size_t nnB = (N + B - 1) / B;
        for (size_t i = 0; i < nB; ++i) {
            auto it = m + h[i].stm;
            if (h[i].ps & 1) {
                fill(it, it + block_length(i), h[i].ps >> 2);
            } else if (h[i].ps & 2) {
                reverse(it, it + block_length(i));
            }
            copy(it, it + block_length(i), buf + st[i]);
        }
        if (nB) copy(buf, buf + N, m);
        nB = (N + B - 1) / B;
        iota(sm, sm + N, 0);
        for (size_t i = 0; i < nB; ++i) st[i] = h[i].stm = i * B;
        st[nB] = N;
        for (size_t i = 0; i < nB; ++i) {
            h[i].ps = 0;
            
            if constexpr(sizeof(T) <= 4) {
                for (size_t j = st[i], k = 0; j < st[i + 1]; ++j) flex[k++] = (uint64_t(j) << 32) | m[j];
                radix_sort<uint64_t, 32>(flex, st[i + 1] - st[i]);
                // sort(flex, flex + st[i + 1] - st[i], [](uint64_t x, uint64_t y){return (x & UINT32_MAX) < (y & UINT32_MAX);});
                for (size_t j = st[i], k = 0; j < st[i + 1]; ++j) sm[j] = flex[k++] >> 32;
            } else {
                sort(sm + st[i], sm + st[i + 1], [&](uint32_t x, uint32_t y) {return m[x] < m[y];});                
            }
            
        }
        times[0] = duration(timeStamp() - stt);
    }

public:
    sqrt_seg_set_leq_rev() = default;
    template<typename I>
    sqrt_seg_set_leq_rev(I first, I last): N(std::distance(first, last)), m(new T[N]), sm(new uint32_t[N]), buf(new T[N]), st(new uint32_t[N + 1]), h(new block[N]) {
        if constexpr(sizeof(T) <= 4) flex = new uint64_t[N];
        copy(first, last, m);
        rebuild(calc_B(1));
    }
    ~sqrt_seg_set_leq_rev() {
        delete[] m, delete[] buf, delete[] sm, delete[] st, delete[] h, delete[] flex;
    }

    template<int type, typename... Params> 
    size_t make_query(Params... params) {
        ++nq;
        chk();
        size_t res = 0;
        auto stt = timeStamp();
        if constexpr(type == 0) {
            res = seg_leq(params...);
        } else if constexpr(type == 1) {
            seg_set(params...);
        } else if constexpr(type == 2) {
            seg_reverse(params...);
        } else {
            assert(0);
        }
        times[type + 1] += duration(timeStamp() - stt);
        return res;
    }
};
