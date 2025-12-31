/*
Push-relabel algorithm for finding min cost flow
Complexity: At most O(V^2Elog(VC))
Important: sum(e.capacity * e.cost | e in edges) * V * 2 should fit into T_cost
*/
template<typename T_flow, typename T_cost>
class push_relabel_mcf {
    static constexpr T_flow INFFLOW = numeric_limits<T_flow>::max();
    static constexpr T_cost INFCOST = numeric_limits<T_cost>::max();
    struct edge {
        int to;
        T_flow f;
        T_cost cost;
        int rev;
        edge(int to, T_flow f, T_cost cost, int rev): to(to), f(f), cost(cost), rev(rev) {}
    };

    T_cost eps = 0;
    int V, source, sink;
    vector<vector<edge>> G;
    vector<size_t> cur;
    vector<T_flow> ex;
    vector<T_cost> h;
    bool flow_calculated = false;

    T_flow max_flow(T_flow flow_limit) {
        int SS = V++;
        G.push_back({edge(source, flow_limit, 0, G[source].size())});
        G[source].emplace_back(SS, 0, 0, G[SS].size() - 1);
        vector<vector<int>> hs(V * 2);
        function<void(edge&, T_flow)> add_flow = [&](edge& e, T_flow f) {
            edge& back = G[e.to][e.rev];
            if (!ex[e.to] && f) hs[h[e.to]].push_back(e.to);
            e.f -= f; ex[e.to] += f;
            back.f += f; ex[back.to] -= f;
        };
        fill(ex.begin(), ex.end(), 0); ex.push_back(0);
        fill(cur.begin(), cur.end(), 0); cur.push_back(0);
        fill(h.begin(), h.end(), 0); h.push_back(0);
        vector<int> co(V * 2);
        h[SS] = V;
        ex[sink] = 1;
        co[0] = V - 1;
        for (auto& e : G[SS]) add_flow(e, e.f);
        for (int hi = 0; hi >= 0 && hs[hi].size();) {
            int u = hs[hi].back();
            hs[hi].pop_back();
            while (ex[u] > 0) {
                if (cur[u] == G[u].size()) {
                    h[u] = 1e9;
                    for (size_t i = 0; i < G[u].size(); ++i) {
                        auto& e = G[u][i];
                        if (e.f && h[u] > h[e.to] + 1) {
                            h[u] = h[e.to] + 1, cur[u] = i;
                        }
                    }
                    if (++co[h[u]], !--co[hi] && hi < V) {
                        for (int i = 0; i < V; ++i) {
                            if (hi < h[i] && h[i] < V) {
                                --co[h[i]];
                                h[i] = V + 1;
                            }
                        }
                    }
                    hi = h[u];
                } else if (G[u][cur[u]].f && h[u] == h[G[u][cur[u]].to] + 1) {
                    add_flow(G[u][cur[u]], min(ex[u], G[u][cur[u]].f));
                } else {
                    ++cur[u];
                }
            }
            while (hi >= 0 && hs[hi].empty()) --hi;
        }
        T_flow flow = -ex[SS];
        --V;
        G.pop_back();
        G[source].pop_back();
        ex.pop_back();
        cur.pop_back();
        h.pop_back();
        return flow;
    }

    void push(edge& e, T_flow amt) {
        if (e.f < amt) amt = e.f;
        e.f -= amt; ex[e.to] += amt;
        G[e.to][e.rev].f += amt; ex[G[e.to][e.rev].to] -= amt;
    }

    void relabel(int v) {
        T_cost nh = -INFCOST / 2;
        for (size_t i = 0; i < G[v].size(); ++i) {
            const edge& e = G[v][i];
            if (e.f && nh < h[e.to] - e.cost) {
                nh = h[e.to] - e.cost;
                cur[v] = i;
            }
        }
        h[v] = nh - eps;
    }

public:
    push_relabel_mcf(size_t V, size_t source, size_t sink): V(V), source(source), sink(sink), G(V), cur(V), ex(V), h(V) {
        assert(source != sink);
        assert(max(source, sink) < V);
    }

    void clear() {
        eps = 0;
        for (auto& i : G) i.clear();
        flow_calculated = false;
    }

    void add_edge(int from, int to, T_flow capacity, T_cost cost, bool is_directed) {
        assert(capacity >= 0);
        assert(0 <= min(from, to) && max(from, to) < V);
        if (from == to) {assert(cost >= 0); return;}
        cost *= V;
        eps = max(eps, abs(cost));
        G[from].emplace_back(to, capacity, cost, G[to].size());
        G[to].emplace_back(from, 0, -cost, G[from].size() - 1);
        if (!is_directed) add_edge(to, from, capacity, cost, 1);
    }

    pair<T_flow, T_cost> calc_min_cost_flow(T_flow flow_limit = INFFLOW / 2, int scale = 1) {
        T_cost cost = 0;
        for (int i = 0; i < V; ++i) {
            for (edge& e : G[i]) cost += e.cost * e.f;
        }
        T_flow flow = max_flow(flow_limit);
        fill(ex.begin(), ex.end(), 0);
        fill(cur.begin(), cur.end(), 0);
        fill(h.begin(), h.end(), 0);
        vector<int> inq(V);
        queue<int> q;
        for (; eps; eps >>= scale) {
            fill(cur.begin(), cur.end(), 0);
            for (int i = 0; i < V; ++i) {
                for (auto& e : G[i]) {
                    if (h[i] + e.cost - h[e.to] < 0 && e.f) push(e, e.f);
                }
            }
            for (int i = 0; i < V; ++i) {
                if (ex[i] > 0) {
                    q.push(i);
                    inq[i] = 1;
                }
            }
            while (!q.empty()) {
                int u = q.front(); q.pop();
                inq[u] = 0;
                while (ex[u] > 0) {
                    if (cur[u] == G[u].size()) relabel(u);
                    for (; cur[u] < G[u].size(); ++cur[u]) {
                        edge& e = G[u][cur[u]];
                        if (h[u] + e.cost - h[e.to] < 0) {
                            push(e, ex[u]);
                            if (ex[e.to] > 0 && inq[e.to] == 0) {
                                q.push(e.to);
                                inq[e.to] = 1;
                            }
                            if (ex[u] == 0) break;
                        }
                    }
                }
            }
            if (eps > 1 && eps >> scale == 0) eps = T_cost(1) << scale;
        }
        for (int i = 0; i < V; ++i) {
            for (edge& e : G[i]) cost -= e.cost * e.f;
        }
        flow_calculated = true;
        return {flow, cost / (V * 2)};
    }
};
