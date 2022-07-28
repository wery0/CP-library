namespace dijkstra {

    #define edge pair<int, T>

    template<typename T>
    void calc_distances_and_predecessors(vec<vec<edge>> &l, const int st, vec<T> &dst, vec<int> &pr, const T inf = numeric_limits<T>::max()) {
        const int a = l.size();
        dst = vec<T>(a, inf);
        auto cmp = [&](const edge & p1, const edge & p2) {return p1.S > p2.S;};
        priority_queue<edge, vec<edge>, decltype(cmp)> pq(cmp);
        dst[st] = 0;
        pr[st] = -1;
        pq.push({st, 0});
        while (!pq.empty()) {
            auto [v, cur] = pq.top(); pq.pop();
            if (cur != dst[v]) continue;
            for (auto &[h, d] : l[v]) {
                if (cur + d < dst[h]) {
                    dst[h] = cur + d;
                    pr[h] = v;
                    pq.push({h, dst[h]});
                }
            }
        }
    }

    template<typename T>
    void calc_distances(vec<vec<edge>> &l, const int st, vec<T> &dst, const T inf = numeric_limits<T>::max()) {
        const int a = l.size();
        dst = vec<T>(a, inf);
        auto cmp = [&](const edge & p1, const edge & p2) {return p1.S > p2.S;};
        priority_queue<edge, vec<edge>, decltype(cmp)> pq(cmp);
        dst[st] = 0;
        pq.push({st, 0});
        while (!pq.empty()) {
            auto [v, cur] = pq.top(); pq.pop();
            if (cur != dst[v]) continue;
            for (auto &[h, d] : l[v]) {
                if (cur + d < dst[h]) {
                    dst[h] = cur + d;
                    pq.push({h, dst[h]});
                }
            }
        }
    }

    template<typename T>
    vec<T> calc_distances(vec<vec<edge>> &l, const int st, const T inf = numeric_limits<T>::max()) {
        vec<T> dst;
        calc_distances<T>(l, st, dst, inf);
        return dst;
    }
};