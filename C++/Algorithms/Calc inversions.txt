namespace inversions_count {
    template<typename T>
    vec<int> compress(vec<T> &m, int first_val = 0) {
        const int a = m.size();
        vec<T> n = m;
        sort(all(n));
        vec<int> ans(a);
        for (int q = 0; q < a; ++q) {
            ans[q] = first_val + lower_bound(all(n), m[q]) - n.begin();
        }
        return ans;
    }

// G must be at least 1 greater than the array size
    template<typename T>
    struct fenv {
        int G;
        vec<T> fen;
        //T fen[G] = {0};
        //array<T, G> fen{};

        fenv() {}

        fenv(int _G) {
            G = _G;
            fen = vec<T>(G);
        }

        fenv(vec<T> &n) {
            G = n.size() + 1;
            fen = vec<T>(G);
            for (int q = 1; q <= n.size(); q++) {
                fen[q] += n[q - 1];
                const int nw = q + (q & -q);
                if (nw < G) fen[nw] += fen[q];
            }
            for (int q = n.size() + 1; q < G; q++) {
                const int nw = q + (q & -q);
                if (nw < G) fen[nw] += fen[q];
            }
        }

        void clear() {
            fill(all(fen), 0);
        }

        void add(int p, T x) {  //0-index
            assert(0 <= p && p < G - 1);
            for (p += 1; p < G; p += p & -p) fen[p] += x;
        }

        T pref_sum(int p) {
            T o = 0;
            for (; p; p -= p & -p) o += fen[p];
            return o;
        }

        T suf_sum(int p) {
            return pref_sum(G - 1) - pref_sum(p);
        }

        T sum_lr(int l, int r) { //0-index
            assert(0 <= l && r < G - 1);
            return pref_sum(r + 1) - pref_sum(l);
        }

        //[1, 2, 3] -> f(4) = 2, f(6) = 3
        int max_pref_with_sum_leq_k(T k) {
            int p = 0, step = 1 << __lg(G);
            T s = 0;
            for (; step; step >>= 1) {
                int nw = p + step;
                if (nw < G && s + fen[nw] <= k) p = nw, s += fen[nw];
            }
            return p;
        }
    };

    template<typename T>
    ll calc_inv(vec<T> &m) {
        const int a = m.size();
        vec<int> n = compress(m);
        fenv <int> fen = fenv<int>(a + 1);
        ll ans = 0;
        for (int q = 0; q < a; ++q) {
            ans += fen.suf_sum(n[q] + 1);
            fen.add(n[q], 1);
        }
        return ans;
    }
};