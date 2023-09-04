//Vantage point tree
//T - type of coordinates
//D - number of dimensions
template<typename T, const int D, typename R = long double>
class VP_tree {
    static_assert(is_signed_v<T>);
    static_assert(is_floating_point_v<R>);

    using point = array<T, D>;
    //Use this instead when you want to store some info with points
    // struct point {
    //     array<T, D> p;
    //     T& operator[](size_t i) {return p[i];}
    //     const T& operator[](size_t i) const {return p[i];}
    // };

    //Change, if need
    static constexpr R EPS = 1e-9;

    size_t n;
    vector<point> m, vps;
    vector<R> vpr;
    point pnt;

    bool check(R x, R y, R z) const {
        return x + y + EPS < z;
    }

    //Using this could be faster when T is integral type, but then dist must return square of euclidean distance.
    // bool check(T x, T y, T z) const {
    //     T c = z - x - y;
    //     return c < 0 ? 0 : (__uint128_t)4 * x * y < (__uint128_t)c * c;
    // }

    template<bool exclude_itself>
    void rec(const point& p, auto f, auto get_interesting_radius, auto ans, int cnt_exc_itself = 0) const {
        int cnt = 0;
        auto go = [&](auto && go, size_t l, size_t r, size_t v) {
            if (l == r) {
                if constexpr(exclude_itself) {
                    if (m[l] == p && ++cnt <= cnt_exc_itself) {return;}
                }
                f(l);
                return;
            }
            size_t md = (l + r) >> 1;
            auto go_left = [&]() {go(go, l, md, v + 1);};
            auto go_right = [&]() {go(go, md + 1, r, v + 2 * (md - l + 1));};
            R df = metric(p, vps[v]), ir = get_interesting_radius();
            if (check(df, ir, vpr[v])) go_left();
            else if (check(vpr[v], ir, df)) go_right();
            else {
                const bool flag = df < vpr[v];
                (flag ? go_left() : go_right());
                ir = get_interesting_radius();
                if (!check(df, ir, vpr[v])) (flag ? go_right() : go_left());
            }
        };
        go(go, 0, n - 1, 0);
    }

    //Euclidean distance. Change, if need.
    R metric(const point& p1, const point& p2) const {
        T res = 0;
        for (size_t i = 0; i < D; ++i) res += (p1[i] - p2[i]) * (p1[i] - p2[i]);
        return sqrt((R)res);
    }

public:
    VP_tree() = default;

    template<typename I>
    VP_tree(I first, I last): n(last - first) {
        if (!n) return;
        m.resize(n);
        for (size_t i = 0; i < n; ++i) m[i] = *(first + i);
        vps.resize(n * 2 - 1);
        vpr.resize(n * 2 - 1);
        mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        const int MAGIC = 0;            //Try some small values, maybe it would help :)
        auto build = [&](auto&& build, size_t l, size_t r, size_t v, size_t dep = 0) {
            if (l == r) return;
            uniform_int_distribution<size_t> gen(l, r);
            size_t md = (l + r) / 2;
            point best = m[gen(rng)];
            T k1 = dep % 4 < 2 ? 1 : -1;
            T k2 = dep % 2 == 1 ? 1 : -1;
            for (int i = 0; i < MAGIC; ++i) {
                point p = m[gen(rng)];
                if (p[0] * k1 + p[1] * k2 > best[0] * k1 + best[1] * k2) best = p;
            }
            vps[v] = best;
            nth_element(m.begin() + l, m.begin() + md, m.begin() + r + 1,
            [&](const point& p1, const point& p2) {return metric(vps[v], p1) < metric(vps[v], p2);});
            vpr[v] = metric(vps[v], m[md]);
            build(build, l, md, v + 1, dep + 1);
            build(build, md + 1, r, v + 2 * (md - l + 1), dep + 1);
        };
        build(build, 0, n - 1, 0);
        pnt = m[0];
        for (const point& p : m) {
            if (p != m[0]) {pnt = p; break;}
        }
    }

    VP_tree(vector<point>& points) {
        *this = VP_tree(points.begin(), points.end());
    }

    point closest_point(const point& p, int cnt_exclude_itself = 0) const {
        if (p == pnt && pnt == m[0]) return p;
        point ans = !cnt_exclude_itself ? m[0] : p != m[0] ? m[0] : pnt;
        auto f = [&](int l) {if (metric(p, m[l]) + EPS < metric(p, ans)) ans = m[l];};
        auto g = [&]() {return metric(p, ans);};
        if (!cnt_exclude_itself) rec<0>(p, f, g, ans);
        else rec<1>(p, f, g, ans, cnt_exclude_itself);
        return ans;
    }

    vector<point> k_closest_points(const point& p, const int k, int cnt_exclude_itself = 0) const {
        auto cmp = [&](const point& p1, const point& p2) {return metric(p, p1) < metric(p, p2);};
        priority_queue<point, vector<point>, decltype(cmp)> pq(cmp);
        auto g = [&]() {return pq.size() < k ? numeric_limits<R>::max() : metric(p, pq.top());};
        auto f = [&](int l) {pq.push(m[l]); if (pq.size() > k) pq.pop();};
        if (!cnt_exclude_itself) rec<0>(p, f, g, pq);
        else rec<1>(p, f, g, pq, cnt_exclude_itself);
        vector<point> ans(pq.size());
        for (size_t i = pq.size(); !pq.empty(); pq.pop()) ans[--i] = pq.top();
        return ans;
    }

    vector<point> all_closest_points(const point& p, int cnt_exclude_itself = 0) const {
        vector<point> ans;
        auto f = [&](int l) {
            if (ans.empty()) {ans = {m[l]}; return;}
            auto d1 = metric(p, m[l]), d2 = metric(p, ans[0]);
            if (d1 + EPS < d2) ans = {m[l]};
            else if (abs(d1 - d2) <= EPS) ans.push_back(m[l]);
        };
        auto g = [&]() {return ans.empty() ? numeric_limits<R>::max() : metric(p, ans[0]);};
        if (!cnt_exclude_itself) rec<0>(p, f, g, ans);
        else rec<1>(p, f, g, ans, cnt_exclude_itself);
        return ans;
    }
};
