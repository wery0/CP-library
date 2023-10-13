template<typename T_flow>
class ford_fulkerson {
    struct edge {
        int from, to;
        T_flow cap;
        T_flow f = 0;

        edge(int from, int to, T_flow cap): from(from), to(to), cap(cap) {}
    };

    int V, ss, tt, us_iter = 0;
    vector<vector<int>> l;
    vector<edge> store;
    vector<int> us;
    bool flow_is_calculated = false;

    T_flow dfs(int v, T_flow scl = 1, T_flow min_flow = numeric_limits<T_flow>::max()) {
        if (v == tt) return min_flow;
        if (us[v] == us_iter) return 0;
        us[v] = us_iter;
        for (int i : l[v]) {
            auto& e = store[i];
            if (e.cap - e.f < scl) continue;
            T_flow res = dfs(e.to, scl, min(min_flow, e.cap - e.f));
            if (res > 0) {
                e.f += res;
                store[i ^ 1].f -= res;
                return res;
            }
        }
        return 0;
    }

public:
    //ss - source, tt - sink
    ford_fulkerson(int V, int ss, int tt): V(V), ss(ss), tt(tt), l(V), us(V) {
        assert(ss != tt);
    }

    void clear() {
        store.clear();
        for (auto& i : l) i.clear();
        flow_is_calculated = false;
    }

    void add_edge(int x, int y, T_flow capacity, bool is_directed) {
        assert(!flow_is_calculated);
        assert(capacity >= 0);
        assert(0 <= min(x, y) && max(x, y) < V);
        l[x].push_back(store.size());
        store.emplace_back(x, y, capacity);
        l[y].push_back(store.size());
        store.emplace_back(y, x, is_directed ? 0 : capacity);
    }

    /*
      Complexity: at most
      - O(EF) without scaling (F is the answer, i. e. max flow)
      - O(E^2log(C)) with scaling (C is max edge capacity)
    */
    T_flow calc_max_flow(bool do_scaling = true) {
        T_flow ans = 0;
        for (T_flow mxf = do_scaling ? numeric_limits<T_flow>::max() : 1; mxf > 0; mxf /= 2) {
            while (true) {
                ++us_iter;
                T_flow res = dfs(ss, mxf);
                if (!res) break;
                ans += res;
            }
        }
        flow_is_calculated = true;
        return ans;
    }

    //For every added edge returns the flow going through it.
    //If the flow on edge x->y is negative, then the flow goes from y to x (could only happen with undirected edges)
    vector<tuple<int, int, T_flow>> get_flow_on_edges() const {
        vector<tuple<int, int, T_flow>> res(store.size() / 2);
        for (size_t i = 0; i < store.size(); i += 2) {
            const auto& e = store[i];
            res[i / 2] = {e.from, e.to, e.f};
        }
        return res;
    }

    //Returns edges (their numbers) that form the min cut
    vector<int> get_min_cut() const {
        assert(flow_is_calculated);
        vector<char> us(V);
        auto dfs = [&](auto&& dfs, int v) -> void {
            us[v] = 1;
            for (int i : l[v]) {
                const auto& e = store[i];
                if (us[e.to]) continue;
                if (e.f < e.cap) dfs(dfs, e.to);
            }
        };
        dfs(dfs, ss);
        vector<int> res;
        for (int v = 0; v < V; ++v) {
            if (!us[v]) continue;
            for (int i : l[v]) {
                const auto& e = store[i];
                if (!us[e.to]) {
                    res.push_back(i / 2);
                }
            }
        }
        sort(res.begin(), res.end());
        return res;
    }

    //Works for directed networks
    //Returns vector of paths from ss to tt
    vector<pair<T_flow, vector<int>>> get_flow_path_decomposition(bool as_vertex_nums) const {
        assert(flow_is_calculated);
        vector<pair<T_flow, vector<int>>> res;
        auto s = store;
        vector<int> us(V);
        int us_iter = 0;
        vector<int> egs;
        auto dfs = [&](auto&& dfs, int v, T_flow min_flow = numeric_limits<T_flow>::max()) -> T_flow {
            if (v == tt) return min_flow;
            if (us[v] == us_iter) return 0;
            us[v] = us_iter;
            for (int i : l[v]) {
                if (i & 1) continue;
                auto& e = s[i];
                if (e.f == 0) continue;
                T_flow res = dfs(dfs, e.to, min(min_flow, e.f));
                if (res > 0) {
                    egs.push_back(i / 2);
                    e.f -= res;
                    s[i ^ 1].f += res;
                    return res;
                }
            }
            return 0;
        };
        while (true) {
            ++us_iter;
            egs.clear();
            T_flow tyt = dfs(dfs, ss);
            if (!tyt) break;
            reverse(egs.begin(), egs.end());
            if (as_vertex_nums) {
                for (int& i : egs) i = store[i * 2].from;
                egs.push_back(tt);
            }
            res.emplace_back(tyt, egs);
        }
        return res;
    }
};
