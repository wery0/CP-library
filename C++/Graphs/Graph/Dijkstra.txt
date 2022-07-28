namespace dijkstra {

    template<typename T, typename D>
    void calc_distances_and_predecessors(graph<edge<T>> &g, const int st, vec<D> &dst, vec<int> &pr, const D inf = numeric_limits<D>::max()) {
        const int a = g.V;
        dst = vec<D>(a, inf);
        auto cmp = [&](const pair<int, D> & p1, const pair<int, D> & p2) {return p1.S > p2.S;};
        priority_queue<pair<int, D>, vec<pair<int, D>>, decltype(cmp)> pq(cmp);
        dst[st] = 0;
        pr[st] = -1;
        pq.push({st, 0});
        while (!pq.empty()) {
            auto [v, cur] = pq.top(); pq.pop();
            if (cur != dst[v]) continue;
            for (auto &[h, d] : g[v]) {
                if (cur + d < dst[h]) {
                    dst[h] = cur + d;
                    pr[h] = v;
                    pq.push({h, dst[h]});
                }
            }
        }
    }

    template<typename T, typename D>
    void calc_distances(graph<edge<T>> &g, const int st, vec<D> &dst, const D inf = numeric_limits<D>::max()) {
        const int a = g.V;
        dst = vec<D>(a, inf);
        auto cmp = [&](const pair<int, D> & p1, const pair<int, D> & p2) {return p1.S > p2.S;};
        priority_queue<pair<int, D>, vec<pair<int, D>>, decltype(cmp)> pq(cmp);
        dst[st] = 0;
        pq.push({st, 0});
        while (!pq.empty()) {
            auto [v, cur] = pq.top(); pq.pop();
            if (cur != dst[v]) continue;
            for (auto &[h, d] : g[v]) {
                if (cur + d < dst[h]) {
                    dst[h] = cur + d;
                    pq.push({h, dst[h]});
                }
            }
        }
    }

    template<typename T, typename D>
    vec<D> calc_distances(graph<edge<T>> &g, const int st, const D inf = numeric_limits<D>::max()) {
        vec<D> dst;
        calc_distances<T, D>(g, st, dst, inf);
        return dst;
    }
}