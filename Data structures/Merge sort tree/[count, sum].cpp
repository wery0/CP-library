//T - type of coordinates and weights of points.
//C - type of answer (sum of all point weights should fit in C)
//Contains two different versions of each function: with fractional cascading and without
template<typename T, typename C>
class merge_sort_tree {

    struct pt {T x, y, w;};

    size_t n, U, M;
    vector<int> lp;
    vector<int> st;
    vector<pt> points;
    vector<T> store_x;
    vector<T> store_y;
    vector<C> store_w;
    bool is_prepared = false;

    void my_merge(size_t v) {
        const size_t L = st[v << 1 | 1] - st[v << 1];
        const size_t R = st[(v + 1) << 1] - st[v << 1 | 1];
        auto yl = store_y.begin() + st[v << 1];
        auto yr = store_y.begin() + st[v << 1 | 1];
        auto wl = store_w.begin() + st[v << 1];
        auto wr = store_w.begin() + st[v << 1 | 1];
        auto itl = lp.begin() + st[v] + v;
        auto ity = store_y.begin() + st[v];
        auto itw = store_w.begin() + st[v];
        for (size_t i = 0, j = 0; i + j < L + R; ++ity, ++itw, ++itl) {
            *itl = i;
            if (j == R || i < L && *yl < *yr) {
                *ity = *yl;
                *itw = wl[i];
                ++i;
                ++yl;
            } else {
                *ity = *yr;
                *itw = wr[j];
                ++j;
                ++yr;
            }
        }
        *itl = L;
    }

    size_t work_cnt(size_t v, T y1, T y2) const {
        auto it_l = lower_bound(store_y.begin() + st[v], store_y.begin() + st[v + 1], y1);
        auto it_r = upper_bound(store_y.begin() + st[v], store_y.begin() + st[v + 1], y2);
        return it_r - it_l;
    }

    C work_sum(size_t v, T y1, T y2) const {
        size_t l = lower_bound(store_y.begin() + st[v], store_y.begin() + st[v + 1], y1) - store_y.begin();
        size_t r = upper_bound(store_y.begin() + st[v], store_y.begin() + st[v + 1], y2) - store_y.begin();
        if (l >= r) return 0;
        return store_w[r - 1] - (l - st[v] ? store_w[l - 1] : 0);
    }

    size_t seg_cnt(size_t ql, size_t qr, size_t l, size_t r, size_t v, size_t ly, size_t ry) const {
        if (qr < l || r < ql || ly >= ry) return 0;
        if (ql <= l && r <= qr) return ry - ly;
        size_t md = (l + r) >> 1;
        auto pl = lp[st[v] + v + ly];
        auto pr = lp[st[v] + v + ry];
        return seg_cnt(ql, qr, l, md, v << 1, pl, pr) +
               seg_cnt(ql, qr, md + 1, r, v << 1 | 1, ly - pl, ry - pr);
    }

    C seg_sum(size_t ql, size_t qr, size_t l, size_t r, size_t v, size_t ly, size_t ry) const {
        if (qr < l || r < ql || ly >= ry) return 0;
        if (ql <= l && r <= qr) {
            return store_w[st[v] + ry - 1] - (ly ? store_w[st[v] + ly - 1] : 0);
        }
        size_t md = (l + r) >> 1;
        auto pl = lp[st[v] + v + ly];
        auto pr = lp[st[v] + v + ry];
        return seg_sum(ql, qr, l, md, v << 1, pl, pr) +
               seg_sum(ql, qr, md + 1, r, v << 1 | 1, ly - pl, ry - pr);
    }

public:
    merge_sort_tree(size_t N = 1) {
        points.reserve(N);
    }

    void clear() {
        points.clear();
        store_x.clear();
        store_y.clear();
        store_w.clear();
        is_prepared = false;
    }

    void add_point(T x, T y, T w = 1) {
        points.emplace_back(x, y, w);
        is_prepared = false;
    }

    //O(nlog(n))
    void prepare() {
        n = points.size();
        U = n & (n - 1) ? 2 << __lg(n) : n;
        M = (n ? __lg(n) + 1 : 0) * n + (n & (n - 1) ? n : 0);
        st.resize(U * 2 + 1);
        store_x.resize(n);
        store_y.resize(M);
        store_w.resize(M);
        lp.resize(M + (n & (n - 1) ? n : 0));
        is_prepared = true;
        if (n == 0) return;
        auto go = [&](auto&& go, size_t l, size_t r, size_t v, size_t dep = 0) -> void {
            st[v] = n * dep + min(l, n);
            if (l == r) return;
            size_t md = (l + r) >> 1;
            go(go, l, md, v << 1, dep + 1);
            go(go, md + 1, r, v << 1 | 1, dep + 1);
        };
        go(go, 0, U - 1, 1);
        st.back() = M;
        sort(points.begin(), points.end(), [](const pt& l, const pt& r) {return l.x < r.x;});
        for (size_t i = 0; i < n; ++i) {
            store_x[i] = points[i].x;
            store_y[st[U + i]] = points[i].y;
            store_w[st[U + i]] = points[i].w;
        }
        for (size_t i = U; --i; ) my_merge(i);
        for (size_t i = U; --i; ) {
            partial_sum(store_w.begin() + st[i],
                        store_w.begin() + st[i + 1],
                        store_w.begin() + st[i]);
        }
    }

    //Counts # points in rectangle [x1, x2] x [y1, y2]
    //O(log(n)^2)
    size_t rect_count(T x1, T y1, T x2, T y2) const {
        assert(is_prepared);
        size_t l = lower_bound(store_x.begin(), store_x.end(), x1) - store_x.begin();
        size_t r = upper_bound(store_x.begin(), store_x.end(), x2) - store_x.begin();
        if (l >= r--) return 0;
        l += U, r += U;
        size_t ans = 0;
        while (l <= r) {
            if (l & 1) ans += work_cnt(l, y1, y2);
            if (~r & 1) ans += work_cnt(r, y1, y2);
            l = (l + 1) >> 1;
            r = (r - 1) >> 1;
        }
        return ans;
    }

    //Counts # points in rectangle [x1, x2] x [y1, y2]
    //O(log(n))
    size_t rect_count_FC(T x1, T y1, T x2, T y2) const {
        assert(is_prepared);
        size_t l = lower_bound(store_x.begin(), store_x.end(), x1) - store_x.begin();
        size_t r = upper_bound(store_x.begin(), store_x.end(), x2) - store_x.begin();
        if (l >= r--) return 0;
        size_t ly = lower_bound(store_y.begin(), store_y.begin() + n, y1) - store_y.begin();
        size_t ry = upper_bound(store_y.begin(), store_y.begin() + n, y2) - store_y.begin();
        return seg_cnt(l, r, 0, U - 1, 1, ly, ry);
    }

    //Counts sum of weights of points in rectangle [x1, x2] x [y1, y2]
    //O(log(n))
    C rect_sum_FC(T x1, T y1, T x2, T y2) const {
        assert(is_prepared);
        if (x1 > x2 || y1 > y2) return 0;
        size_t l = lower_bound(store_x.begin(), store_x.end(), x1) - store_x.begin();
        size_t r = upper_bound(store_x.begin(), store_x.end(), x2) - store_x.begin();
        if (l >= r--) return 0;
        size_t ly = lower_bound(store_y.begin(), store_y.begin() + n, y1) - store_y.begin();
        size_t ry = upper_bound(store_y.begin(), store_y.begin() + n, y2) - store_y.begin();
        return seg_sum(l, r, 0, U - 1, 1, ly, ry);
    }

    //Counts sum of weights of points in rectangle [x1, x2] x [y1, y2]
    //O(log(n)^2)
    C rect_sum(T x1, T y1, T x2, T y2) const {
        assert(is_prepared);
        if (x1 > x2 || y1 > y2) return 0;
        size_t l = lower_bound(store_x.begin(), store_x.end(), x1) - store_x.begin();
        size_t r = upper_bound(store_x.begin(), store_x.end(), x2) - store_x.begin();
        if (l >= r--) return 0;
        l += U, r += U;
        C ans = 0;
        while (l <= r) {
            if (l & 1) ans += work_sum(l, y1, y2);
            if (~r & 1) ans += work_sum(r, y1, y2);
            l = (l + 1) >> 1;
            r = (r - 1) >> 1;
        }
        return ans;
    }
};