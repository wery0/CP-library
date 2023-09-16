template<typename T_flow>
class ford_fulkerson {
    struct edge {
        int from, to;
        T_flow f;

        edge(int from, int to, T_flow f): from(from), to(to), f(f) {}
    };

    int V, ss, tt, us_iter = 0;
    vector<vector<int>> l;
    vector<edge> store;
    vector<int> us;

    T_flow dfs(int v, T_flow scl = 1, T_flow min_flow = numeric_limits<T_flow>::max()) {
        if (v == tt) return min_flow;
        if (us[v] == us_iter) return 0;
        us[v] = us_iter;
        for (int i : l[v]) {
            auto& e = store[i];
            if (e.f < scl) continue;
            T_flow res = dfs(e.to, scl, min(min_flow, e.f));
            if (res > 0) {
                e.f -= res;
                store[i ^ 1].f += res;
                return res;
            }
        }
        return 0;
    }

public:
    ford_fulkerson(int V, int ss, int tt): V(V), ss(ss), tt(tt), l(V), us(V) {}

    void add_edge(int x, int y, T_flow capacity, bool is_directed) {
        assert(0 <= min(x, y) && max(x, y) < V);
        l[x].push_back(store.size());
        store.emplace_back(x, y, capacity);
        l[y].push_back(store.size());
        store.emplace_back(y, x, is_directed ? 0 : capacity);
    }

    T_flow calc(bool do_scaling = true) {
        T_flow ans = 0;
        for (T_flow mxf = do_scaling ? MAX_EDGE_CAPACITY : 1; mxf > 0; mxf /= 2) {
            while (true) {
                ++us_iter;
                T_flow res = dfs(ss, mxf);
                if (!res) break;
                ans += res;
            }
        }
        return ans;
    }
};
