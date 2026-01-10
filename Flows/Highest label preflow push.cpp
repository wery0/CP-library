/*
Highest label preflow push (HLPP) algorithm for finding max flow
Complexity: At most O(V^2sqrt(E)), fast on practice
*/
template<typename T_flow>
class highest_label_preflow_push {
    static constexpr T_flow INFFLOW = numeric_limits<T_flow>::max();
    struct edge {
        int to;
        T_flow flow, cap;
        edge(int to, T_flow flow, T_flow cap): to(to), flow(flow), cap(cap) {}
    };

    int V, source, sink;
    int mxh = 0, work = 0;
    vector<edge> store;
    vector<vector<int>> l, lst, gap;
    vector<T_flow> ex;
    vector<int> h, cnt;
    bool flow_calculated = false;

    void updh(int v, int nh) {
        work++;
        if (h[v] != V) cnt[h[v]]--;
        h[v] = nh;
        if (nh == V) return;
        cnt[nh]++, mxh = nh;
        gap[nh].push_back(v);
        if (ex[v] > 0) lst[nh].push_back(v);
    }

    void global_relabel() {
        work = 0;
        fill(h.begin(), h.end(), V);
        fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < min(mxh, V); ++i) lst[i].clear(), gap[i].clear();
        h[sink] = 0;
        queue<int> q({sink});
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int i : l[v]) {
                auto& e = store[i];
                if (h[e.to] == V && store[i ^ 1].cap - store[i ^ 1].flow > 0) {
                    q.push(e.to), updh(e.to, h[v] + 1);
                }
            }
            mxh = h[v];
        }
    }

    void push(int v, int i) {
        auto& e = store[i];
        if (ex[e.to] == 0) lst[h[e.to]].push_back(e.to);
        T_flow df = min(ex[v], e.cap - e.flow);
        e.flow += df, store[i ^ 1].flow -= df;
        ex[v] -= df, ex[e.to] += df;
    }

    void discharge(int v) {
        int nh = V;
        for (int i : l[v]) {
            edge& e = store[i];
            if (e.cap - e.flow > 0) {
                if (h[v] == h[e.to] + 1) {
                    push(v, i);
                    if (ex[v] <= 0) return;
                } else {
                    nh = min(nh, h[e.to] + 1);
                }
            }
        }
        if (cnt[h[v]] > 1) {
            updh(v, nh);
        } else {
            for (int i = h[v]; i <= mxh; ++i) {
                for (auto j : gap[i]) updh(j, V);
                gap[i].clear();
            }
        }
    }

public:
    highest_label_preflow_push(size_t V, size_t source, size_t sink): V(V), source(source), sink(sink), l(V), lst(V + 1), gap(V), ex(V), h(V), cnt(V) {
        assert(source != sink);
        assert(max(source, sink) < V);
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

    void clear() {
        store.clear();
        for (auto& i : l) i.clear();
        for (auto& i : lst) i.clear();
        for (auto& i : gap) i.clear();
        mxh = work = 0;
        fill(h.begin(), h.end(), 0);
        fill(cnt.begin(), cnt.end(), 0);
        fill(ex.begin(), ex.end(), 0);
        flow_calculated = false;
    }

    T_flow calc_max_flow(bool cancel_excessive_preflow, int heur_n = -1) {
        assert(!flow_calculated);
        if (heur_n == -1) heur_n = V;
        function<T_flow(T_flow)> go = [&](T_flow limit) -> T_flow {
            fill(ex.begin(), ex.end(), 0);
            ex[source] = limit;
            global_relabel();
            for (int i : l[source]) push(source, i);
            for (; mxh >= 0; --mxh) {
                while (!lst[mxh].empty()) {
                    int v = lst[mxh].back();
                    lst[mxh].pop_back();
                    discharge(v);
                    if (work > heur_n * 4) global_relabel();
                }
            }
            return ex[sink];
        };
        T_flow max_flow = go(INFFLOW);
        if (cancel_excessive_preflow) {
            mxh = work = 0;
            fill(h.begin(), h.end(), 0);
            fill(cnt.begin(), cnt.end(), 0);
            for (auto& i : lst) i.clear();
            for (auto& i : gap) i.clear();
            for (size_t i = 0; i < store.size(); i += 2) {
                store[i ^ 1].flow += store[i].flow;
                store[i].flow = 0;
            }
            go(max_flow);
        }
        flow_calculated = true;
        return max_flow;
    }
};
