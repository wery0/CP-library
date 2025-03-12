//Finds the sum of weights of points in rectangle.
//T - type of coordinates and weights of points.
//C - type of answer (sum of all point weights should fit in C)
template<typename T, typename C>
class static_offline_rectangle_sum {

    struct pt {T x, y, w;};
    struct item {T x, y1, y2, n;};

    vector<pt> points;
    vector<item> queries;

public:
    static_offline_rectangle_sum(size_t N = 1, size_t Q = 1) {
        points.reserve(N);
        queries.reserve(Q * 2);
    }

    void clear() {points.clear(); queries.clear();}

    void add_point(T x, T y, T w = 1) {
        points.emplace_back(x, y, w);
    }

    void add_query(T x1, T y1, T x2, T y2) {
        if (x1 > x2 || y1 > y2) y1 = y2 + 1;
        size_t n = queries.size() / 2 + 1;
        queries.emplace_back(x1 - 1, y1, y2, -n);
        queries.emplace_back(x2, y1, y2, n);
    }

    //O((N + Q)log(N))
    vector<C> process_queries() {
        const size_t N = points.size();
        const size_t Q = queries.size();
        sort(queries.begin(), queries.end(), [](const item& l, const item& r) {return l.x < r.x;});
        sort(points.begin(), points.end(), [](const pt& l, const pt& r) {return l.x < r.x;});
        vector<T> ystore(N);
        vector<C> ans(Q / 2);
        for (size_t i = 0; i < N; ++i) ystore[i] = points[i].y;
        sort(ystore.begin(), ystore.end());
        ystore.erase(unique(ystore.begin(), ystore.end()), ystore.end());
        const size_t sz = ystore.size();
        vector<C> fen(sz + 1);
        for (size_t j = 0; auto [qx, qy1, qy2, qn] : queries) {
            for (; j < N && points[j].x <= qx; ++j) {
                T y = lower_bound(ystore.begin(), ystore.end(), points[j].y) - ystore.begin();
                for (size_t p = y + 1; p <= sz; p += p & -p) fen[p] += points[j].w;
            }
            qy1 = lower_bound(ystore.begin(), ystore.end(), qy1) - ystore.begin();
            qy2 = upper_bound(ystore.begin(), ystore.end(), qy2) - ystore.begin();
            C sm = 0;
            if (qy1 <= qy2) {
                for (size_t p = qy1; p; p -= p & -p) sm -= fen[p];
                for (size_t p = qy2; p; p -= p & -p) sm += fen[p];
            }
            ans[abs(qn) - 1] += sm * (qn < 0 ? -1 : 1);
        }
        return ans;
    }
};
