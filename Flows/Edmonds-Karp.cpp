/*
Edmonds-Karp algorithm for finding max flow
Idea: same as Ford-Fulkerson but with bfs instead of dfs
Complexity: at most
- min(VE^2, EF) without scaling (F is max flow)
- O(E^2log(C)) with scaling (C is max edge capacity)
*/
template<typename T_flow>
class edmonds_karp {
    static constexpr T_flow INFFLOW = numeric_limits<T_flow>::max();
    struct edge {
        int to;
        T_flow flow, cap;
        edge(int to, T_flow flow, T_flow cap): to(to), flow(flow), cap(cap) {}
    };

    int V, source, sink;
    vector<vector<int>> l;
    vector<edge> store;
    vector<int> pr;
    vector<T_flow> min_flow;
    bool flow_calculated = false;

    T_flow bfs(int v, T_flow scl = 1) {
        fill(pr.begin(), pr.end(), -1);
        fill(min_flow.begin(), min_flow.end(), INFFLOW);
        deque<int> dq = {source};
        --min_flow[source];
        while (dq.size()) {
            int v = dq.front(); dq.pop_front();
            for (int i : l[v]) {
                auto& e = store[i];
                if (e.cap - e.flow < scl) continue;
                if (min_flow[e.to] == INFFLOW) {
                    min_flow[e.to] = min(min_flow[v], e.cap - e.flow);
                    pr[e.to] = i;
                    dq.emplace_back(e.to);
                }
            }
        }
        if (min_flow[sink] == INFFLOW) return 0;
        for (int v = sink; v != source; ) {
            int i = pr[v];
            store[i].flow += min_flow[sink];
            store[i ^ 1].flow -= min_flow[sink];
            v = store[i ^ 1].to;
        }
        return min_flow[sink];
    }

public:
    edmonds_karp(size_t V, size_t source, size_t sink): V(V), source(source), sink(sink), l(V), pr(V), min_flow(V) {
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
        store.emplace_back(to, 0, capacity);
        l[to].push_back(store.size());
        store.emplace_back(from, 0, is_directed ? 0 : capacity);
    }

    T_flow calc_max_flow(bool do_scaling = true) {
        assert(!flow_calculated);
        T_flow ans = 0;
        for (T_flow mxf = do_scaling ? INFFLOW : 1; mxf > 0; mxf /= 2) {
            while (true) {
                T_flow res = bfs(source, mxf);
                if (!res) break;
                ans += res;
            }
        }
        flow_calculated = true;
        return ans;
    }
};
