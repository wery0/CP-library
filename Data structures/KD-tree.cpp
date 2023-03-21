template<typename T, const int D>
class KD_tree {
    static_assert(is_signed_v<T>);
    using point = array<T, D>;

    size_t n;
    vector<point> m;
    vector<T> mid_axis;

    void rec(const point& p, auto f, auto get_interesting_radius, auto ans, int k = 1) const {
        auto go = [&](auto&& go, size_t l, size_t r, size_t v, size_t axis = 0) {
            if (l == r) {
                f(l);
                return;
            }
            axis -= axis == D ? D : 0;
            size_t md = (l + r) >> 1;
            auto go_left = [&]() {go(go, l, md, v + 1, axis + 1);};
            auto go_right = [&]() {go(go, md + 1, r, v + 2 * (md - l + 1), axis + 1);};
            T df = p[axis] - mid_axis[v];
            (df < 0 ? go_left() : go_right());
            if (get_interesting_radius() >= df * df) (df < 0 ? go_right() : go_left());   //Use df instead of df * df when dist is not squared.
        };
        go(go, 0, n - 1, 0);
    }

    //Euclidean distance squared. Change, if need.
    T dist(const point& p1, const point& p2) const {
        T res = 0;
        for (size_t i = 0; i < D; ++i) res += (p1[i] - p2[i]) * (p1[i] - p2[i]);
        return res;
    }

public:
    KD_tree() = default;

    template<typename I>
    KD_tree(I first, I last): n(last - first) {
        if (!n) return;
        m.resize(n);
        for (size_t i = 0; i < n; ++i) m[i] = *(first + i);
        mid_axis.resize(n * 2 - 1);
        auto build = [&](auto&& build, size_t l, size_t r, size_t v, size_t axis = 0) {
            if (l == r) return;
            axis -= axis == D ? D : 0;
            size_t md = (l + r) / 2;
            nth_element(m.begin() + l, m.begin() + md, m.begin() + r + 1,
            [&](const point & a1, const point & a2) {return a1[axis] < a2[axis];});
            mid_axis[v] = m[md][axis];
            build(build, l, md, v + 1, axis + 1);
            build(build, md + 1, r, v + 2 * (md - l + 1), axis + 1);
        };
        build(build, 0, n - 1, 0);
    }

    KD_tree(vector<point>& points) {
        *this = KD_tree(points.begin(), points.end());
    }

    point closest_point(const point& p) const {
        point ans = m[0];
        auto f = [&](int l) {if (dist(p, m[l]) < dist(p, ans)) ans = m[l];};
        auto g = [&]() {return dist(p, ans);};
        rec(p, f, g, ans);
        return ans;
    }

    vector<point> k_closest_points(const int k, const point& p) const {
        auto cmp = [&](const point& p1, const point& p2) {return dist(p, p1) < dist(p, p2);};
        priority_queue<point, vector<point>, decltype(cmp)> pq(cmp);
        auto g = [&]() {return pq.size() < k ? numeric_limits<T>::max() : dist(p, pq.top());};
        auto f = [&](int l) {pq.push(m[l]); if (pq.size() > k) pq.pop();};
        rec(p, f, g, pq, k);
        vector<point> ans(pq.size());
        for (size_t i = pq.size(); !pq.empty(); pq.pop()) ans[--i] = pq.top();
        return ans;
    }

    vector<point> all_closest_points(const point& p) const {
        vector<point> ans;
        auto f = [&](int l) {
            if (ans.empty()) {ans = {m[l]}; return;}
            auto d1 = dist(p, m[l]), d2 = dist(p, ans[0]);
            if (d1 < d2) ans = {m[l]};
            else if (d1 == d2) ans.push_back(m[l]);
        };
        auto g = [&]() {return ans.empty() ? numeric_limits<T>::max() : dist(p, ans[0]);};
        rec(p, f, g, ans);
        return ans;
    }
};