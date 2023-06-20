//T - type of coordinates and weights of points
//C - type of answer (sum of all point weights should fit in C)
//Contains two different versions of each function: with fractional cascading and without
//Before making queries, add all points and call prepare()
template<typename T, typename C>
class merge_sort_tree {

    struct pt {
        T x, y, w;
        pt(T x, T y, T w) : x(x), y(y), w(w) {}
        bool operator<(const pt& rhs) const { return x < rhs.x || (x == rhs.x && y < rhs.y); }
    };

    size_t n, U, M;
    vector<int> lp;
    vector<int> st;
    vector<pt> points;
    vector<T> store_x;
    vector<T> store_y;
    vector<C> store_w;
    fenwick<C> fenw;

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

    void _prepare() {
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
        sort(points.begin(), points.end());
        for (size_t i = 0; i < n; ++i) {
            store_x[i] = points[i].x;
            store_y[st[U + i]] = points[i].y;
            store_w[st[U + i]] = points[i].w;
        }
        for (size_t i = U; --i;) my_merge(i);
        fenw = fenwick<C>(store_w.begin(), store_w.end());
    }

    template<typename A>
    A straightforward(T x1, T y1, T x2, T y2, A neutral_element, auto merge, auto f) const {
        assert(is_prepared);
        size_t l = lower_bound(store_x.begin(), store_x.end(), x1) - store_x.begin();
        size_t r = upper_bound(store_x.begin(), store_x.end(), x2) - store_x.begin();
        if (l >= r--) return neutral_element;
        l += U, r += U;
        auto work = [&](size_t v) {
            size_t l = lower_bound(store_y.begin() + st[v], store_y.begin() + st[v + 1], y1) - store_y.begin();
            size_t r = upper_bound(store_y.begin() + st[v], store_y.begin() + st[v + 1], y2) - store_y.begin();
            if (l >= r--) return neutral_element;
            return f(v, l, r);
        };
        A ans = neutral_element;
        while (l <= r) {
            if (l & 1) ans = merge(ans, work(l));
            if (~r & 1) ans = merge(ans, work(r));
            l = (l + 1) >> 1;
            r = (r - 1) >> 1;
        }
        return ans;
    }

    template<typename A>
    A seg_go(size_t ql, size_t qr, size_t l, size_t r, size_t v, size_t ly, size_t ry, A neutral_element, auto merge, auto f) const {
        if (qr < l || r < ql || ly >= ry) return neutral_element;
        if (ql <= l && r <= qr) return f(v, st[v] + ly, st[v] + ry - 1);
        size_t md = (l + r) >> 1;
        auto pl = lp[st[v] + v + ly];
        auto pr = lp[st[v] + v + ry];
        return merge(seg_go(ql, qr, l, md, v << 1, pl, pr, neutral_element, merge, f),
                     seg_go(ql, qr, md + 1, r, v << 1 | 1, ly - pl, ry - pr, neutral_element, merge, f));
    }

    template<typename A>
    A fractional_cascading(T x1, T y1, T x2, T y2, A neutral_element, auto merge, auto f) const {
        assert(is_prepared);
        size_t l = lower_bound(store_x.begin(), store_x.end(), x1) - store_x.begin();
        size_t r = upper_bound(store_x.begin(), store_x.end(), x2) - store_x.begin();
        if (l >= r--) return neutral_element;
        size_t ly = lower_bound(store_y.begin(), store_y.begin() + n, y1) - store_y.begin();
        size_t ry = upper_bound(store_y.begin(), store_y.begin() + n, y2) - store_y.begin();
        return seg_go(l, r, 0, U - 1, 1, ly, ry, neutral_element, merge, f);
    }

    void point_add(size_t pos, T val) {
        size_t l = 0, r = U - 1, v = 1;
        while (true) {
            size_t py = lower_bound(store_y.begin() + st[v], store_y.begin() + st[v + 1], points[pos].y) - store_y.begin() - st[v];
            fenw.point_add(st[v] + py, val);
            if (l == r) break;
            size_t md = (l + r) >> 1;
            v <<= 1;
            if (pos <= md) r = md;
            else l = md + 1, v |= 1;
        }
    }

    void point_add_FC(size_t pos, size_t py, T val) {
        size_t l = 0, r = U - 1, v = 1;
        while (true) {
            fenw.point_add(st[v] + py, val);
            if (l == r) break;
            size_t md = (l + r) >> 1;
            auto pl = lp[st[v] + v + py];
            v <<= 1;
            if (pos <= md) r = md, py = pl;
            else l = md + 1, v |= 1, py -= pl;
        }
    }

public:
    merge_sort_tree(size_t N = 1) { points.reserve(N); }

    //O(nlog(n))
    void prepare() { _prepare(); }

    void add_point(T x, T y, T w = 1) {
        points.emplace_back(x, y, w);
        is_prepared = false;
    }

    void clear() {
        points.clear();
        store_x.clear();
        store_y.clear();
        store_w.clear();
        is_prepared = false;
    }

    //Counts # points in rectangle [x1, x2] x [y1, y2]
    //O(log(n)) with FC, O(log(n)^2) without
    size_t rect_count(T x1, T y1, T x2, T y2, bool use_fractional_cascading = true) const {
        using A = size_t;
        auto merge = [](const A& x, const A& y) { return x + y; };
        auto f = [&](size_t v, size_t l, size_t r) { return r - l + 1; };
        return use_fractional_cascading ?
               fractional_cascading<A>(x1, y1, x2, y2, 0, merge, f) :
               straightforward<A>(x1, y1, x2, y2, 0, merge, f);
    }

    //Counts sum of weights of points in rectangle [x1, x2] x [y1, y2]
    //O(log(n)^2)
    C rect_sum(T x1, T y1, T x2, T y2, bool use_fractional_cascading = true) const {
        using A = C;
        auto merge = [](const A& x, const A& y) { return x + y; };
        auto f = [&](size_t v, size_t l, size_t r) { return fenw.seg_sum(l, r); };
        return use_fractional_cascading ?
               fractional_cascading<A>(x1, y1, x2, y2, 0, merge, f) :
               straightforward<A>(x1, y1, x2, y2, 0, merge, f);
    }

    //Point (x, y) must already exist!
    //O(log(n)) with FC, O(log(n)^2) without
    void point_add(T x, T y, T val, bool use_fractional_cascading = true) {
        assert(is_prepared);
        auto it = lower_bound(points.begin(), points.end(), pt{x, y, -1});
        assert(it != points.end());
        auto [px, py, pw] = *it;
        assert(px == x && py == y);
        if (use_fractional_cascading) {
            size_t ly = lower_bound(store_y.begin(), store_y.begin() + n, y) - store_y.begin();
            point_add_FC(it - points.begin(), ly, val);
        } else {
            point_add(it - points.begin(), val);
        }
    }
};