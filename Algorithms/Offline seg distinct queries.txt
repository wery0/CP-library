struct offline_seg_distinct_queries {

    struct fenv {
        int G;
        vec<int> fen;

        fenv() {}

        fenv(int _G) {
            G = _G;
            fen.resize(G);
        }

        fenv(vec<int> &n) {
            G = n.size() + 1;
            fen.resize(G);
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

        void point_add(int p, int x) {
            for (p += 1; p < G; p += p & -p) fen[p] += x;
        }

        int pref_sum(int p) {
            int o = 0;
            for (p += 1; p; p -= p & -p) o += fen[p];
            return o;
        }

        int suf_sum(int p) {
            return pref_sum(G - 2) - pref_sum(p - 1);
        }

        int seg_sum(int l, int r) {
            return pref_sum(r) - pref_sum(l - 1);
        }
    };

    int a;
    vec<int> closest_left_eq;
    fenv fen;

    template<typename I>
    vec<int> get_closest_left_eq(I first, I last) {
        using T = typename iterator_traits<I>::value_type;
        int a = last - first;
        vec<int> ans(a, -1);
        umap<T, int> mp;
        auto cit = first;
        for (int q = 0; q < a; ++q, ++cit) {
            T cur = *cit;
            if (mp.count(cur)) {
                ans[q] = mp[cur];
            }
            mp[cur] = q;
        }
        return ans;
    }

    template<typename T>
    offline_seg_distinct_queries(vec<T> &n) {
        a = n.size();
        closest_left_eq = get_closest_left_eq(all(n));
    }

    vec<int> process_queries(vec<pii> _que) {
        int z = _que.size();
        vec<int> ans(z);
        vec<array<int, 3>> que(z);
        for (int q = 0; q < z; ++q) que[q] = {_que[q].F, _que[q].S, q};
        sort(all(que), [&](const auto & l, const auto & r) {return l[1] < r[1];});
        fen = fenv(a + 1);
        for (int r = 0, i = 0; r < a; r++) {
            fen.point_add(r, 1);
            if (closest_left_eq[r] != -1) fen.point_add(closest_left_eq[r], -1);
            for (; i < z && que[i][1] == r; ++i) {
                ans[que[i][2]] = fen.seg_sum(que[i][0], que[i][1]);
            }
        }
        return ans;
    }
};