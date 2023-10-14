template<typename T_flow>
class HLPP {
    static constexpr T_flow INF = numeric_limits<T_flow>::max();
    struct edge {
        int to;
        T_flow f = 0, cap;

        edge() = default;
        edge(int to, T_flow cap): to(to), cap(cap) {}
    };

    int V, ss, tt;
    int mxh = 0, work = 0;
    vector<edge> store;
    vector<vector<int>> adj;
    vector<vector<int>> lst, gap;
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
        h[tt] = 0;
        queue<int> q({tt});
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int i : adj[v]) {
                auto& e = store[i];
                if (h[e.to] == V && store[i ^ 1].cap - store[i ^ 1].f > 0) {
                    q.push(e.to), updh(e.to, h[v] + 1);
                }
            }
            mxh = h[v];
        }
    }

    void push(int v, int i) {
        auto& e = store[i];
        if (ex[e.to] == 0) lst[h[e.to]].push_back(e.to);
        T_flow df = min(ex[v], e.cap - e.f);
        e.f += df, store[i ^ 1].f -= df;
        ex[v] -= df, ex[e.to] += df;
    }

    void discharge(int v) {
        int nh = V;
        for (int i : adj[v]) {
            edge& e = store[i];
            if (e.cap - e.f > 0) {
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
    HLPP(int n, int ss, int tt): V(n), ss(ss), tt(tt), adj(V), lst(V + 1), gap(V), ex(V), h(V), cnt(V) {
        assert(ss != tt);
    }

    void add_edge(int x, int y, T_flow capacity, bool is_directed) {
        assert(!flow_calculated);
        assert(capacity >= 0);
        assert(0 <= min(x, y) && max(x, y) < V);
        adj[x].push_back(store.size());
        store.emplace_back(y, capacity);
        adj[y].push_back(store.size());
        store.emplace_back(x, is_directed ? 0 : capacity);
    }

    void clear() {
        store.clear();
        for (int i = 0; i < V; ++i) {
            adj[i].clear();
            lst[i].clear();
            gap[i].clear();
        }
        mxh = work = 0;
        fill(h.begin(), h.end(), 0);
        fill(cnt.begin(), cnt.end(), 0);
        fill(ex.begin(), ex.end(), 0);
        flow_calculated = false;
    }

    T_flow calc_max_flow(int heur_n = -1) {
        if (heur_n == -1) heur_n = V;
        fill(ex.begin(), ex.end(), 0);
        ex[ss] = INF;
        global_relabel();
        for (int i : adj[ss]) push(ss, i);
        for (; mxh >= 0; --mxh) {
            while (!lst[mxh].empty()) {
                int v = lst[mxh].back();
                lst[mxh].pop_back();
                discharge(v);
                if (work > 4 * heur_n) global_relabel();
            }
        }
        flow_calculated = true;
        return ex[tt];
    }

    //For every added edge returns the flow going through it.
    //If the flow on edge x->y is negative, then the flow goes from y to x (could only happen with undirected edges)
    //Will work only if ex[ss] was set to max_flow at the beginning of calc_max_flow() since this push-relabel maintains preflow
    vector<tuple<int, int, T_flow>> get_flow_on_edges() const {
        vector<tuple<int, int, T_flow>> res(store.size() / 2);
        for (size_t i = 0; i < store.size(); i += 2) {
            const auto& e = store[i];
            res[i / 2] = {store[i ^ 1].to, e.to, e.f};
        }
        return res;
    }
};
