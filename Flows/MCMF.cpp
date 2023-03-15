//Implementation of SPFA algorithm
template<typename T_flow, typename T_cost>
struct min_cost_max_flow {
    struct edge {
        int fr, to;
        T_flow f, c;
        T_cost w;
    };

    int n, ss, tt;
    vector<vector<int>> l;
    vector<edge> e;

    min_cost_max_flow(int n, int ss, int tt): n(n), ss(ss), tt(tt), l(n) {}

    //Edge is directed
    void add_edge(int fr, int to, T_flow c, T_cost w) {
        l[fr].push_back(e.size());
        e.emplace_back(fr, to, 0, c, w);
        l[to].push_back(e.size());
        e.emplace_back(to, fr, 0, 0, -w);
    }

    pair<T_flow, T_cost> calc() {
        const T_cost inf = numeric_limits<T_cost>::max();
        T_flow flow = 0;
        T_cost cost = 0;
        vector<T_cost> dst(n);
        vector<T_flow> mn(n);
        vector<int> pr(n), inq(n);
        while (true) {
            fill(dst.begin(), dst.end(), inf);
            fill(pr.begin(), pr.end(), -1);
            fill(inq.begin(), inq.end(), 0);
            fill(mn.begin(), mn.end(), numeric_limits<T_flow>::max());
            dst[ss] = 0;
            inq[ss] = 1;
            deque<int> dq = {ss};
            while (!dq.empty()) {
                // constexpr size_t MAGIC = 2;
                // for (size_t i = 0; i < MAGIC && i + 1 < dq.size(); ++i) {
                //     if (dst[dq.back()] < dst[dq[i]]) {
                //         swap(dq.back(), dq[i]);
                //         break;
                //     }
                // }
                int v = dq.front(); dq.pop_front();
                inq[v] = 0;
                for (int i : l[v]) {
                    if (e[i].c == e[i].f) continue;
                    T_cost tyt = dst[v] + e[i].w;
                    int to = e[i].to;
                    if (tyt < dst[to]) {
                        dst[to] = tyt;
                        pr[to] = i;
                        mn[to] = min(mn[e[i].fr], e[i].c - e[i].f);
                        if (!inq[to]) {
                            inq[to] = 1;
                            dq.push_back(to);
                        }
                    }
                }
            }
            if (dst[tt] == inf) break;
            T_flow tyt = mn[tt];
            flow += tyt;
            cost += dst[tt] * tyt;
            for (int v = tt; v != ss; ) {
                const int i = pr[v];
                e[i].f += tyt;
                e[i ^ 1].f -= tyt;
                v = e[i].fr;
            }
        }
        return {flow, cost};
    }
};