/*
Malhotra, Pramodh-Kumar and Maheshwari algorithm for finding max flow
Idea: build layered network and find blocking flow in it by finding the vertex with minimum x = min(incoming, outgoing) capacity and pushing x flow through it.
Complexity: at most O(V^3)
*/
template<typename T_flow>
class malhotra_pramodh_maheshvari {
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
    vector<int> ptr, ptr2;
    vector<T_flow> pin, pout, to_push;
    bool flow_calculated = false;

    void build_layered_network() {
        fill(layer.begin(), layer.end(), V);
        deque<int> dq = {source};
        layer[source] = 0;
        while (dq.size()) {
            int v = dq.front(); dq.pop_front();
            for (int i : l[v]) {
                auto& e = store[i];
                if (e.cap - e.flow == 0) continue;
                if (layer[e.to] == V) {
                    layer[e.to] = layer[v] + 1;
                    dq.emplace_back(e.to);
                }
            }
        }
    }

    T_flow find_blocking_flow() {
        fill(pin.begin(), pin.end(), 0);
        fill(pout.begin(), pout.end(), 0);
        fill(ptr.begin(), ptr.end(), 0);
        fill(ptr2.begin(), ptr2.end(), 0);
        for (int i = 0; i < store.size(); ++i) {
            int from = store[i ^ 1].to, to = store[i].to;
            if (layer[to] == layer[from] + 1 && (layer[to] < layer[sink] || to == sink)) {
                pin[to] += store[i].cap - store[i].flow;
                pout[from] += store[i].cap - store[i].flow;
            }
        }
        vector<int> eso; eso.reserve(V - 2);
        for (int v = 0; v < V; ++v) {
            if (v != source && v != sink) eso.push_back(v);
        }
        vector<int> banned(store.size());
        function<void(int)> remove_node = [&](int v) {
            for (int u = min(ptr[v], ptr2[v]); u < l[v].size(); ++u) {
                int i = l[v][u];
                if (banned[i]) continue;
                int from = v, to = store[i].to;
                if (layer[to] == layer[from] + 1 && (layer[to] < layer[sink] || to == sink)) {
                    pin[to] -= store[i].cap - store[i].flow;
                    pout[from] -= store[i].cap - store[i].flow;
                    banned[i] = 1;
                    banned[i ^ 1] = 1;
                }
                if (layer[from] == layer[to] + 1 && (layer[from] < layer[sink] || from == sink)) {
                    pin[from] -= store[i ^ 1].cap - store[i ^ 1].flow;
                    pout[to] -= store[i ^ 1].cap - store[i ^ 1].flow;
                    banned[i ^ 1] = 1;
                    banned[i] = 1;
                }
            }
        };
        auto push_flow = [&](int i, T_flow f) {
            int from = store[i ^ 1].to, to = store[i].to;
            store[i].flow += f;
            store[i ^ 1].flow -= f;
            pin[to] -= f;
            pout[from] -= f;
        };
        T_flow flow = 0;
        for (int i : l[source]) {
            if (store[i].to == sink && store[i].flow < store[i].cap) {
                flow += store[i].cap - store[i].flow;
                push_flow(i, store[i].cap - store[i].flow);
            }
        }
        for (int ii = 0; eso.size();) {
            T_flow pmin = INFFLOW;
            int bv = -1;
            ii %= eso.size();
            for (int i = 0; i < eso.size() && pmin > 0; ++i, ++ii) {
                if (ii == eso.size()) ii = 0;
                int v = eso[ii];
                if (min(pin[v], pout[v]) < pmin) {
                    pmin = min(pin[v], pout[v]);
                    bv = ii;
                }
            }
            swap(eso[bv], eso.back()); bv = eso.back(); eso.pop_back();
            if (pmin == 0) {
                remove_node(bv);
                continue;
            }
            flow += pmin;
            to_push[bv] = pmin;
            deque<int> dq = {bv};
            to_push[source] = to_push[sink] = 0;
            while (dq.size()) {
                int v = dq.front(); dq.pop_front();
                for (; ptr[v] < l[v].size(); ++ptr[v]) {
                    int i = l[v][ptr[v]];
                    if (banned[i]) continue;
                    auto& e = store[i];
                    T_flow mn = min(e.cap - e.flow, to_push[v]);
                    if (mn == 0) continue;
                    if (layer[e.to] != layer[v] + 1 || (layer[e.to] >= layer[sink] && e.to != sink)) continue;
                    if (to_push[e.to] == 0) dq.push_back(e.to);
                    to_push[v] -= mn;
                    to_push[e.to] += mn;
                    push_flow(i, mn);
                    if (!to_push[v]) break;
                    banned[i] = 1;
                }
                assert(to_push[v] == 0 || v == sink);
            }
            to_push[bv] = pmin;
            dq = {bv};
            while (dq.size()) {
                int v = dq.front(); dq.pop_front();
                for (; ptr2[v] < l[v].size(); ++ptr2[v]) {
                    int i = l[v][ptr2[v]];
                    if (banned[i ^ 1]) continue;
                    auto& e = store[i];
                    T_flow mn = min(store[i ^ 1].cap - store[i ^ 1].flow, to_push[v]);
                    if (mn == 0) continue;
                    if (layer[e.to] != layer[v] - 1) continue;
                    if (to_push[e.to] == 0) dq.push_back(e.to);
                    to_push[v] -= mn;
                    to_push[e.to] += mn;
                    push_flow(i ^ 1, mn);
                    if (!to_push[v]) break;
                    banned[i ^ 1] = 1;
                }
                assert(to_push[v] == 0 || v == source);
            }
            remove_node(bv);
        }
        return flow;
    }

public:
    malhotra_pramodh_maheshvari(size_t V, size_t source, size_t sink): V(V), source(source), sink(sink), l(V), layer(V), ptr(V), ptr2(V), pin(V), pout(V), to_push(V) {
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

    T_flow calc_max_flow() {
        assert(!flow_calculated);
        T_flow ans = 0;
        while (true) {
            build_layered_network();
            if (layer[sink] == V) break;
            T_flow flow = find_blocking_flow();
            ans += flow;
            assert(flow > 0);
        }
        flow_calculated = true;
        return ans;
    }
};
