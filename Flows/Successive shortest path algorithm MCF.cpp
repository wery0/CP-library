/*
Successive shortest path algorithm (SSPA) with SPFA for finding min cost flow
Complexity: At most O(SPFA(V, E)min(F, flow_limit)) (F is max flow)
Problems:
https://cses.fi/problemset/task/2121 (min cost k flow)
https://cses.fi/problemset/task/2129 (assignment problem)
https://cses.fi/problemset/task/2130 (min cost k flow + flow path decomposition)
*/
template<typename T_flow, typename T_cost>
class sspa_mcf {
    static constexpr T_flow INFFLOW = numeric_limits<T_flow>::max();
    static constexpr T_cost INFCOST = numeric_limits<T_cost>::max();
    struct edge {
        int to;
        T_flow flow, cap;
        T_cost cost;
        edge(int to, T_flow flow, T_flow cap, T_cost cost): to(to), flow(flow), cap(cap), cost(cost) {}
    };

    int V, source, sink;
    vector<vector<int>> l;
    vector<edge> store;
    bool flow_calculated = false;

public:
    sspa_mcf(size_t V, size_t source, size_t sink): V(V), source(source), sink(sink), l(V) {
        assert(source != sink);
        assert(max(source, sink) < V);
    }

    void clear() {
        store.clear();
        for (auto& i : l) i.clear();
        flow_calculated = false;
    }

    void add_edge(int from, int to, T_flow capacity, T_cost cost, bool is_directed) {
        assert(capacity >= 0);
        assert(0 <= min(from, to) && max(from, to) < V);
        if (from == to) {assert(cost >= 0); return;}
        l[from].push_back(store.size());
        store.emplace_back(to, 0, capacity, cost);
        l[to].push_back(store.size());
        store.emplace_back(from, 0, 0, -cost);
        if (!is_directed) add_edge(to, from, capacity, cost, 1);
    }

    pair<T_flow, T_cost> calc_min_cost_flow(T_flow flow_limit = INFFLOW / 2) {
        assert(!flow_calculated);
        T_flow flow = 0;
        T_cost cost = 0;
        vector<T_cost> dst(V);
        vector<T_flow> mn(V);
        vector<int> pr(V), inq(V);
        while (flow < flow_limit) {
            fill(dst.begin(), dst.end(), INFCOST);
            fill(pr.begin(), pr.end(), -1);
            fill(inq.begin(), inq.end(), 0);
            fill(mn.begin(), mn.end(), numeric_limits<T_flow>::max());
            dst[source] = 0;
            inq[source] = 1;
            deque<int> dq = {source};
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
                    const auto& e = store[i];
                    if (e.cap - e.flow == 0) continue;
                    T_cost tyt = dst[v] + e.cost;
                    int to = e.to;
                    if (tyt < dst[to]) {
                        dst[to] = tyt;
                        pr[to] = i;
                        mn[to] = min(mn[v], e.cap - e.flow);
                        if (!inq[to]) {
                            inq[to] = 1;
                            dq.push_back(to);
                        }
                    }
                }
            }
            if (dst[sink] == INFCOST) break;
            T_flow tyt = min(mn[sink], flow_limit - flow);
            flow += tyt;
            cost += dst[sink] * tyt;
            for (int v = sink; v != source; ) {
                const int i = pr[v];
                store[i].flow += tyt;
                store[i ^ 1].flow -= tyt;
                v = store[i ^ 1].to;
            }
        }
        flow_calculated = true;
        return {flow, cost};
    }
};
