namespace SPFA {

    template<typename T>
    void calc_distances_and_predecessors(vec<vec<pair<int, T>>> &l, const int st, vec<T> &dst, vec<int> &pr, const T inf = numeric_limits<T>::max()) {
        const int a = l.size();
        dst = vec<T>(a, inf);
        vec<bool> inq(a);
        dst[st] = 0;
        pr[st] = -1;
        deque<int> que = {st};
        inq[st] = 1;
        while (!que.empty()) {
            int v = que.front(); que.pop_front();
            const T cur = dst[v];
            inq[v] = 0;
            for (auto &[h, d] : l[v]) {
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

    template<typename T>
    void calc_distances(vec<vec<pair<int, T>>> &l, const int st, vec<T> &dst, const T inf = numeric_limits<T>::max()) {
        const int a = l.size();
        dst = vec<T>(a, inf);
        vec<bool> inq(a);
        dst[st] = 0;
        deque<int> que = {st};
        inq[st] = 1;
        while (!que.empty()) {
            int v = que.front(); que.pop_front();
            const T cur = dst[v];
            inq[v] = 0;
            for (auto &[h, d] : l[v]) {
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

    template<typename T>
    vec<T> calc_distances(vec<vec<pair<int, T>>> &l, const int st, const T inf = numeric_limits<T>::max()) {
        vec<T> dst;
        calc_distances<T>(l, st, dst, inf);
        return dst;
    }
};