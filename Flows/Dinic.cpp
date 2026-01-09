/*
Dinic algorithm for finding max flow
Idea: build layered network and find blocking flow in it by going from source to sink while you can
Complexity:
  General case: at most
    - O(V^2E) without scaling
    - O(VElog(C)) with scaling (C is max edge capacity)
  Unit networks case (comes from Karzanov's theorems): at most
    - O(Esqrt(V)) for networks where for each vertex (except source and sink) there is either incoming or outgoing edge is unique
    - min(O(Esqrt(E)), O(EV^{2/3})) for networks where number of incoming and outgoing edges is unbounded
*/
template<typename T_flow>
class dinic {
    static constexpr T_flow INFFLOW = numeric_limits<T_flow>::max();
    struct edge {
        int to;
        T_flow flow, cap;
        edge(int to, T_flow flow, T_flow cap): to(to), flow(flow), cap(cap) {}
    };

    int V, source, sink;
    vector<vector<int>> l;
    vector<edge> store;
    vector<int> layer;
    vector<int> ptr;
    vector<char> in_layered_network;
    bool flow_calculated = false;

    void build_layered_network(T_flow scl) {
        fill(layer.begin(), layer.end(), V);
        fill(in_layered_network.begin(), in_layered_network.end(), 0);
        deque<int> dq = {source};
        layer[source] = 0;
        while (dq.size()) {
            int v = dq.front(); dq.pop_front();
            for (int i : l[v]) {
                auto& e = store[i];
                if (e.cap - e.flow < scl) continue;
                if (layer[e.to] == V) {
                    layer[e.to] = layer[v] + 1;
                    dq.emplace_back(e.to);
                }
                if (layer[e.to] == layer[v] + 1) {
                    in_layered_network[i] = 1;
                }
            }
        }
    }

    T_flow find_blocking_flow(T_flow scl) {
        fill(ptr.begin(), ptr.end(), 0);
        function<T_flow(int, T_flow)> dfs = [&](int v, T_flow min_flow) -> T_flow {
            if (v == sink) return min_flow;
            for (; ptr[v] < l[v].size(); ++ptr[v]) {
                int i = l[v][ptr[v]];
                if (!in_layered_network[i]) continue;
                auto& e = store[i];
                if (e.cap - e.flow < scl) continue;
                T_flow res = dfs(e.to, min(min_flow, e.cap - e.flow));
                if (res > 0) {
                    e.flow += res;
                    store[i ^ 1].flow -= res;
                    return res;
                }
            }
            return 0;
        };
        T_flow flow = 0;
        while (true) {
            T_flow res = dfs(source, INFFLOW);
            if (res == 0) break;
            flow += res;
        }
        return flow;
    }

public:
    dinic(size_t V, size_t source, size_t sink): V(V), source(source), sink(sink), l(V), layer(V), ptr(V) {
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

    T_flow calc_max_flow(bool do_scaling) {
        assert(!flow_calculated);
        in_layered_network.resize(store.size());
        T_flow ans = 0;
        for (T_flow mxf = do_scaling ? INFFLOW : 1; mxf > 0; mxf /= 2) {
            while (true) {
                build_layered_network(mxf);
                if (layer[sink] == V) break;
                ans += find_blocking_flow(mxf);
            }
        }
        flow_calculated = true;
        return ans;
    }
};
