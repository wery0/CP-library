namespace bfs {

    void go(vec<vec<int>> &l, const int st, vec<int> &dst, const int inf = numeric_limits<int>::max()) {
        const int a = l.size();
        dst.resize(a); fill(all(dst), inf);
        dst[st] = 0;
        deque<int> dq = {st};
        while (!dq.empty()) {
            int v = dq.front(); dq.pop_front();
            for (int h : l[v]) {
                if (dst[h] == inf) {
                    dst[h] = dst[v] + 1;
                    dq.pb(h);
                }
            }
        }
    }

    vec<int> go(vec<vec<int>> &l, const int st, const int inf = numeric_limits<int>::max()) {
        vec<int> dst;
        go(l, st, dst, inf);
        return dst;
    }
};