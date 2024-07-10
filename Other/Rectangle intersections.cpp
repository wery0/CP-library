template<typename T>
class rectangle_intersections_counting {
    template<typename U>
    class fenwick {
        int n;
        vector<U> fen;

    public:
        fenwick() = default;
        fenwick(int n): n(n + 1), fen(n + 1) {}

        void clear() {
            fill(fen.begin(), fen.end(), 0);
        }

        U operator[](int p) const {
            U res = 0;
            for (++p; p < n; p += p & -p) res += fen[p];
            return res;
        }

        void pref_add(int p, U x) {
            for (++p; p; p -= p & -p) fen[p] += x;
        }

        void suf_add(int p, U x) {
            pref_add(n - 2, x);
            pref_add(p - 1, -x);
        }
    };

    struct rect {
        T x1, y1, x2, y2;
        size_t n;
    };

    struct query {
        T p, l, r, sign;
        size_t n;
    };

    vector<rect> rect_store;

    vector<rect> compress() const {
        vector<int> nx(rect_store.size() * 2);
        vector<int> ny(rect_store.size() * 2);
        for (size_t q = 0; const auto& [x1, y1, x2, y2, n] : rect_store) {
            nx[q] = x1;
            nx[q | 1] = x2;
            ny[q] = y1;
            ny[q | 1] = y2;
            q += 2;
        }
        sort(nx.begin(), nx.end());
        sort(ny.begin(), ny.end());
        nx.erase(unique(nx.begin(), nx.end()), nx.end());
        ny.erase(unique(ny.begin(), ny.end()), ny.end());
        vector<rect> new_m(rect_store.size());
        for (size_t q = 0; auto [x1, y1, x2, y2, n] : rect_store) {
            x1 = lower_bound(nx.begin(), nx.end(), x1) - nx.begin();
            x2 = lower_bound(nx.begin(), nx.end(), x2) - nx.begin();
            y1 = lower_bound(ny.begin(), ny.end(), y1) - ny.begin();
            y2 = lower_bound(ny.begin(), ny.end(), y2) - ny.begin();
            new_m[q++] = {x1, y1, x2, y2, n};
        }
        return new_m;
    }

public:
    rectangle_intersections_counting() = default;

    void clear() {rect_store.clear();}

    void add_rect(T x1, T y1, T x2, T y2) {
        rect_store.emplace_back(x1, y1, x2, y2, rect_store.size());
    }

    //For each rectangle, counts the number of other rectangles with which there is an intersection by a positive area.
    //Coordinates of rectangle endpoints are lattice points, i. e. rect(0, 0, 3, 3) has area 9.
    //O(nlog(n))
    vector<int> calc() const {
        const int n = rect_store.size();
        if (n == 0) return {};
        int all_segs = 0;
        fenwick<int> fen_pref(n * 2);
        fenwick<int> fen_suf(n * 2);
        auto add_seg = [&](int l, int r) -> void {
            fen_pref.suf_add(r, 1);
            fen_suf.pref_add(l, 1);
            ++all_segs;
        };
        auto rem_seg = [&](int l, int r) -> void {
            fen_pref.suf_add(r, -1);
            fen_suf.pref_add(l, -1);
            --all_segs;
        };
        auto cnt_intersections = [&](int l, int r) -> int {
            return all_segs - fen_pref[l] - fen_suf[r];
        };
        auto m = compress();
        sort(m.begin(), m.end(), [](const auto& r1, const auto& r2) {return r1.x1 < r2.x1;});
        vector<int> ans(n, -1);
        vector<vector<pair<T, T>>> del_seg(n * 2);
        for (int x = 0, l = 0; l < n; ++x) {
            while (!del_seg[x].empty()) {
                auto [l, r] = del_seg[x].back();
                del_seg[x].pop_back();
                rem_seg(l, r);
            }
            if (m[l].x1 != x) continue;
            int r = l;
            for (; r + 1 < n && m[r + 1].x1 == x;) ++r;
            for (int w = l; w <= r; ++w) ans[m[w].n] += cnt_intersections(m[w].y1, m[w].y2);
            for (int w = l; w <= r; ++w) {
                add_seg(m[w].y1, m[w].y2);
                del_seg[m[w].x2].emplace_back(m[w].y1, m[w].y2);
            }
            l = r + 1;
        }
        vector<query> store;
        for (const auto& [x1, y1, x2, y2, n] : m) {
            store.emplace_back(x1, y1, y2, 1, n);
            store.emplace_back(x2, y1, y2, -1, n);
        }
        sort(store.begin(), store.end(), [](const auto& q1, const auto& q2) {return q1.p > q2.p;});
        reverse(m.begin(), m.end());
        all_segs = 0;
        fen_pref.clear();
        fen_suf.clear();
        for (int x = n * 2, l = 0, ql = 0; l < n || ql < store.size(); --x) {
            int r = l - 1;
            while (r + 1 < n && m[r + 1].x1 == x) ++r;
            for (int w = l; w <= r; ++w) add_seg(m[w].y1, m[w].y2);
            for (; ql < store.size() && store[ql].p == x; ++ql) {
                ans[store[ql].n] += cnt_intersections(store[ql].l, store[ql].r) * store[ql].sign;
            }
            l = r + 1;
        }
        return ans;
    }
};