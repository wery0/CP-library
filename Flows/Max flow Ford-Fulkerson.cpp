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
    ford_fulkerson(int V, int ss, int tt): V(V), ss(ss), tt(tt), l(V), us(V) {}

    void clear() {
        store.clear();
        for (auto& i : l) i.clear(); 
    }

    void add_edge(int x, int y, T_flow capacity, bool is_directed) {
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

    
    //For every added edge returns the flow going through it.
    //If the flow on edge x->y is negative, then the flow goes from y to x (could only happen with undirected edges)
    vector<array<T_flow, 3>> get_flow_on_edges() {
        vector<array<T_flow, 3>> res(store.size() / 2);
        for (size_t i = 0; i < store.size(); i += 2) {
            auto& e = store[i];
            res[i / 2] = {e.from, e.to, e.f};
        }
        return res;
    }
};
