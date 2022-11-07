template<typename T_flow, typename T_cost, const int V>
struct min_cost_max_flow {
    struct edge {
        int fr, to;
        T_flow f;
        T_cost c, w;
    };

    int ss = V - 2, tt = V - 1;
    vector<int> l[V];
    vector<edge> e;

    void add_edge(int fr, int to, T_flow c, T_cost w) {
        l[fr].push_back(e.size());
        e.emplace_back(fr, to, 0, c, w);
        l[to].push_back(e.size());
        e.emplace_back(to, fr, 0, 0, -w);
    }

    vector<T_cost> dst;
    vector<int> pr, inq;
    pair<T_flow, T_cost> calc() {
        const T_cost inf = numeric_limits<T_cost>::max();
        T_flow flow = 0;
        T_cost cost = 0;
        dst.resize(V);
        pr.resize(V);
        inq.resize(V);
        for (;;) {
            fill(all(dst), inf);
            fill(all(pr), -1);
            fill(all(inq), 0);
            dst[ss] = 0;
            inq[ss] = 1;
            deque<int> dq = {ss};
            for (; dq.size();) {
                // constexpr int MAGIC = 1;
                // for (int q = 0; q < MAGIC && q + 1 < dq.size(); ++q) {
                //     if (dst[dq.back()] < dst[dq[q]]) {
                //         swap(dq.back(), dq[q]);
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
                        if (!inq[to]) {
                            inq[to] = 1;
                            dq.push_back(to);
                        }
                    }
                }
            }
            if (dst[tt] == inf) break;
            ++flow;
            cost += dst[tt];
            int v = tt;
            for (; v != ss;) {
                int i = pr[v];
                e[i].f++;
                --e[i ^ 1].f;
                v = e[i].fr;
            }
        }
        return {flow, cost};
    }
};