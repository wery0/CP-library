namespace SPFA {

    template<typename T, typename D>
    void calc_distances_and_predecessors(graph<edge<T>> &g, const int st, vec<D> &dst, vec<int> &pr, const D inf = numeric_limits<D>::max()) {
        const int a = g.V;
        dst = vec<D>(a, inf);
        vec<bool> inq(a);
        dst[st] = 0;
        pr[st] = -1;
        deque<int> que = {st};
        inq[st] = 1;
        while (!que.empty()) {
            constexpr int MAGIC = 1;
            for (int q = 0; q < MAGIC && q + 1 < que.size(); ++q) {
                if (dst[que.back()] < dst[que[q]]) {
                    swap(que.back(), que[q]);
                    break;
                }
            }
            int v = que.front(); que.pop_front();
            const D cur = dst[v];
            inq[v] = 0;
            for (auto &[h, d] : g[v]) {
                if (cur + d < dst[h]) {
                    dst[h] = cur + d;
                    pr[h] = v;
                    if (!inq[h]) {
                        inq[h] = 1;
                        que.pb(h);
                    }
                }
            }
        }
    }

    template<typename T, typename D>
    void calc_distances(graph<edge<T>> &g, const int st, vec<D> &dst, const D inf = numeric_limits<D>::max()) {
        const int a = g.V;
        dst = vec<D>(a, inf);
        vec<bool> inq(a);
        dst[st] = 0;
        deque<int> que = {st};
        inq[st] = 1;
        while (!que.empty()) {
            constexpr int MAGIC = 1;
            for (int q = 0; q < MAGIC && q + 1 < que.size(); ++q) {
                if (dst[que.back()] < dst[que[q]]) {
                    swap(que.back(), que[q]);
                    break;
                }
            }
            int v = que.front(); que.pop_front();
            const D cur = dst[v];
            inq[v] = 0;
            for (auto &[h, d] : g[v]) {
                if (cur + d < dst[h]) {
                    dst[h] = cur + d;
                    if (!inq[h]) {
                        inq[h] = 1;
                        que.pb(h);
                    }
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