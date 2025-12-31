/*
Ford-Fulkerson algorithm for finding max flow
Idea: while there are augmenting path in residual graph, find it and push flow through it
Complexity: at most
- O(EF) without scaling (F is max flow)
- O(E^2log(C)) with scaling (C is max edge capacity)
*/
template<typename T_flow>
class ford_fulkerson {
    struct edge {
        int from, to;
        T_flow flow, cap;
        edge(int from, int to, T_flow flow, T_flow cap): from(from), to(to), flow(flow), cap(cap) {}
    };

    int V, source, sink, us_iter = 0;
    vector<vector<int>> l;
    vector<edge> store;
    vector<int> us;
    bool flow_calculated = false;

    T_flow dfs(int v, T_flow scl = 1, T_flow min_flow = numeric_limits<T_flow>::max()) {
        if (v == sink) return min_flow;
        if (us[v] == us_iter) return 0;
        us[v] = us_iter;
        for (int i : l[v]) {
            auto& e = store[i];
            if (e.cap - e.flow < scl) continue;
            T_flow res = dfs(e.to, scl, min(min_flow, e.cap - e.flow));
            if (res > 0) {
                e.flow += res;
                store[i ^ 1].flow -= res;
                return res;
            }
        }
        return 0;
    }

public:
    ford_fulkerson(size_t V, size_t source, size_t sink): V(V), source(source), sink(sink), l(V), us(V) {
        assert(source != sink);
        assert(max(source, sink) < V);
    }

    void clear() {
        store.clear();
        for (auto& i : l) i.clear();
        flow_calculated = false;
    }

    void add_edge(int from, int to, T_flow capacity, bool is_directed) {
        assert(!flow_calculated);
        assert(capacity >= 0);
        assert(0 <= min(from, to) && max(from, to) < V);
        l[from].push_back(store.size());
        store.emplace_back(from, to, 0, capacity);
        l[to].push_back(store.size());
        store.emplace_back(to, from, 0, is_directed ? 0 : capacity);
    }

    T_flow calc_max_flow(bool do_scaling = true) {
        T_flow ans = 0;
        for (T_flow mxf = do_scaling ? numeric_limits<T_flow>::max() : 1; mxf > 0; mxf /= 2) {
            while (true) {
                ++us_iter;
                T_flow res = dfs(source, mxf);
                if (!res) break;
                ans += res;
            }
        }
        flow_calculated = true;
        return ans;
    }

    //For every added edge returns the flow going through it.
    //If the flow on edge x->y is negative, then the flow goes from y to x (could only happen with undirected edges)
    vector<tuple<int, int, T_flow>> get_flow_on_edges() const {
        vector<tuple<int, int, T_flow>> res(store.size() / 2);
        for (size_t i = 0; i < store.size(); i += 2) {
            const auto& e = store[i];
            res[i / 2] = {e.from, e.to, e.flow};
        }
        return res;
    }

    //Returns edges (their numbers) that form the min cut
    vector<int> get_min_cut(const bool are_edges_directed) const {
        assert(flow_calculated);
        vector<char> us(V);
        auto dfs = [&](auto&& dfs, int v) -> void {
            us[v] = 1;
            for (int i : l[v]) {
                const auto& e = store[i];
                if (us[e.to]) continue;
                if (e.flow < e.cap) dfs(dfs, e.to);
            }
        };
        dfs(dfs, source);
        vector<int> res;
        for (int v = 0; v < V; ++v) {
            if (!us[v]) continue;
            for (int i : l[v]) {
                const auto& e = store[i];
                if (!us[e.to] && (!are_edges_directed || (~i & 1))) {
                    res.push_back(i / 2);
                }
            }
        }
        sort(res.begin(), res.end());
        return res;
    }

    //Works for directed networks
    //Returns vector of paths from source to sink
    vector<pair<T_flow, vector<int>>> get_flow_path_decomposition(bool as_vertex_nums) const {
        assert(flow_calculated);
        vector<pair<T_flow, vector<int>>> res;
        auto s = store;
        vector<int> us(V);
        int us_iter = 0;
        vector<int> egs, ptr(V);
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
                for (int& i : egs) i = store[i * 2].from;
                egs.push_back(sink);
            }
            res.emplace_back(tyt, egs);
        }
        return res;
    }
};
