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
        T_flow flow;
        T_cost cost;
        edge(int to, T_flow flow, T_cost cost): to(to), flow(flow), cost(cost) {}
    };

    T_cost eps = 0;
    int V, source, sink;
    vector<edge> store;
    vector<vector<int>> l;
    vector<size_t> cur;
    vector<T_flow> ex;
    vector<T_cost> h;
    bool flow_calculated = false;

    T_flow max_flow(T_flow flow_limit) {
        int supersource = V++;
        l.push_back({int(store.size())});
        store.emplace_back(source, flow_limit, 0);
        l[source].emplace_back(store.size());
        store.emplace_back(supersource, 0, 0);
        vector<vector<int>> hs(V * 2);
        function<void(int, T_flow)> add_flow = [&](int i, T_flow f) {
            edge& e = store[i];
            edge& back = store[i ^ 1];
            if (!ex[e.to] && f) hs[h[e.to]].push_back(e.to);
            e.flow -= f; ex[e.to] += f;
            back.flow += f; ex[back.to] -= f;
        };
        fill(ex.begin(), ex.end(), 0); ex.push_back(0);
        fill(cur.begin(), cur.end(), 0); cur.push_back(0);
        fill(h.begin(), h.end(), 0); h.push_back(0);
        vector<int> co(V * 2);
        h[supersource] = V;
        ex[sink] = 1;
        co[0] = V - 1;
        for (int i : l[supersource]) add_flow(i, store[i].flow);
        for (int hi = 0; hi >= 0 && hs[hi].size();) {
            int u = hs[hi].back();
            hs[hi].pop_back();
            while (ex[u] > 0) {
                if (cur[u] == l[u].size()) {
                    h[u] = 1e9;
                    for (size_t i = 0; i < l[u].size(); ++i) {
                        auto& e = store[l[u][i]];
                        if (e.flow && h[u] > h[e.to] + 1) {
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
                } else if (auto& e = store[l[u][cur[u]]]; e.flow && h[u] == h[e.to] + 1) {
                    add_flow(l[u][cur[u]], min(ex[u], e.flow));
                } else {
                    ++cur[u];
                }
            }
            while (hi >= 0 && hs[hi].empty()) --hi;
        }
        T_flow flow = -ex[supersource];
        --V;
        l.pop_back();
        l[source].pop_back();
        store.pop_back();
        store.pop_back();
        ex.pop_back();
        cur.pop_back();
        h.pop_back();
        return flow;
    }

    void push(int i, T_flow amt) {
        edge& e = store[i];
        edge& r = store[i ^ 1];
        if (e.flow < amt) amt = e.flow;
        e.flow -= amt; ex[e.to] += amt;
        r.flow += amt; ex[r.to] -= amt;
    }

    void relabel(int v) {
        T_cost nh = -INFCOST / 2;
        for (size_t i = 0; i < l[v].size(); ++i) {
            const edge& e = store[l[v][i]];
            if (e.flow && nh < h[e.to] - e.cost) {
                nh = h[e.to] - e.cost;
                cur[v] = i;
            }
        }
        h[v] = nh - eps;
    }

public:
    push_relabel_mcf(size_t V, size_t source, size_t sink): V(V), source(source), sink(sink), l(V), cur(V), ex(V), h(V) {
        assert(source != sink);
        assert(max(source, sink) < V);
    }

    void clear() {
        eps = 0;
        for (auto& i : l) i.clear();
        flow_calculated = false;
    }

    void add_edge(int from, int to, T_flow capacity, T_cost cost, bool is_directed) {
        assert(capacity >= 0);
        assert(0 <= min(from, to) && max(from, to) < V);
        if (from == to) {assert(cost >= 0); return;}
        cost *= V;
        eps = max(eps, abs(cost));
        l[from].push_back(store.size());
        store.emplace_back(to, capacity, cost);
        l[to].push_back(store.size());
        store.emplace_back(from, 0, -cost);
        if (!is_directed) add_edge(to, from, capacity, cost, 1);
    }

    pair<T_flow, T_cost> calc_min_cost_flow(T_flow flow_limit = INFFLOW / 2, int scale = 1) {
        assert(!flow_calculated);
        T_cost cost = 0;
        for (const auto& e : store) cost += e.cost * e.flow; 
        T_flow flow = max_flow(flow_limit);
        fill(ex.begin(), ex.end(), 0);
        fill(cur.begin(), cur.end(), 0);
        fill(h.begin(), h.end(), 0);
        vector<int> inq(V);
        queue<int> q;
        for (; eps; eps >>= scale) {
            fill(cur.begin(), cur.end(), 0);
            for (int v = 0; v < V; ++v) {
                for (int i : l[v]) {
                    edge& e = store[i];
                    if (h[v] + e.cost - h[e.to] < 0 && e.flow) push(i, e.flow);
                }
            }
            for (int v = 0; v < V; ++v) {
                if (ex[v] > 0) {
                    q.push(v);
                    inq[v] = 1;
                }
            }
            while (!q.empty()) {
                int u = q.front(); q.pop();
                inq[u] = 0;
                while (ex[u] > 0) {
                    if (cur[u] == l[u].size()) relabel(u);
                    for (; cur[u] < l[u].size(); ++cur[u]) {
                        edge& e = store[l[u][cur[u]]];
                        if (h[u] + e.cost - h[e.to] < 0) {
                            push(l[u][cur[u]], ex[u]);
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
        for (const auto& e : store) cost -= e.cost * e.flow; 
        flow_calculated = true;
        return {flow, cost / (V * 2)};
    }

    //For every added edge returns the flow going through it.
    //If the flow on edge x->y is negative, then the flow goes from y to x (could only happen with undirected edges)
    vector<tuple<int, int, T_flow, T_cost>> get_flow_on_edges() const {
        vector<tuple<int, int, T_flow, T_cost>> res(store.size() / 2);
        for (size_t i = 0; i < store.size(); i += 2) {
            const auto& e = store[i];
            res[i / 2] = {store[i ^ 1].to, e.to, store[i ^ 1].flow, e.cost};
        }
        return res;
    }

    //Works for directed networks
    //Returns vector of paths from source to sink
    vector<pair<T_flow, vector<int>>> get_flow_path_decomposition(bool as_vertex_nums) const {
        assert(flow_calculated);
        vector<pair<T_flow, vector<int>>> res;
        vector<int> us(V), ptr(V), egs;
        auto s = store;
        for (int i = 0; i < s.size(); i += 2) {
            swap(s[i].flow, s[i ^ 1].flow);
        }
        int us_iter = 0;
        auto dfs = [&](auto&& dfs, int v, T_flow min_flow = numeric_limits<T_flow>::max()) -> T_flow {
            if (v == sink) return min_flow;
            if (us[v] == us_iter) return 0;
            us[v] = us_iter;
            ptr[v] = 0;
            for (int cnt = 0; cnt < l[v].size(); ++cnt) {
                int i = l[v][ptr[v]++];
                if (ptr[v] == l[v].size()) ptr[v] = 0;
                if (i & 1) continue;
                auto& e = s[i];
                if (e.flow == 0) continue;
                T_flow res = dfs(dfs, e.to, min(min_flow, e.flow));
                if (res > 0) {
                    egs.push_back(i / 2);
                    e.flow -= res;
                    s[i ^ 1].flow += res;
                    return res;
                }
            }
            return 0;
        };
        while (true) {
            ++us_iter;
            egs.clear();
            T_flow tyt = dfs(dfs, source);
            if (!tyt) break;
            reverse(egs.begin(), egs.end());
            if (as_vertex_nums) {
                for (int& i : egs) i = store[i * 2 ^ 1].to;
                egs.push_back(sink);
            }
            res.emplace_back(tyt, egs);
        }
        return res;
    }
};
