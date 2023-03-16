template<typename T>
class HLPP {
    const T INF = numeric_limits<T>::max();
    struct edge {
        int to, rev;
        T f;

        edge() = default;
        edge(int x, int y, T c): to(x), rev(y), f(c) {}
    };

    int V, s, t, mxh, work;
    vector<vector<edge>> adj;
    vector<vector<int>> lst, gap;
    vector<T> ex;
    vector<int> h, cnt;

    void updh(int v, int nh) {
        work++;
        if (h[v] != V) cnt[h[v]]--;
        h[v] = nh;
        if (nh == V) return;
        cnt[nh]++, mxh = nh;
        gap[nh].push_back(v);
        if (ex[v] > 0) lst[nh].push_back(v);
    }

    void globalRelabel() {
        work = 0;
        fill(h.begin(), h.end(), V);
        fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < min(mxh, V); i++) lst[i].clear(), gap[i].clear();
        h[t] = 0;
        queue<int> q({t});
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto& e : adj[v]) {
                if (h[e.to] == V && adj[e.to][e.rev].f > 0) {
                    q.push(e.to), updh(e.to, h[v] + 1);
                }
            }
            mxh = h[v];
        }
    }

    void push(int v, edge& e) {
        if (ex[e.to] == 0) lst[h[e.to]].push_back(e.to);
        T df = min(ex[v], e.f);
        e.f -= df, adj[e.to][e.rev].f += df;
        ex[v] -= df, ex[e.to] += df;
    }

    void discharge(int v) {
        int nh = V;
        for (auto& e : adj[v]) {
            if (e.f > 0) {
                if (h[v] == h[e.to] + 1) {
                    push(v, e);
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
    HLPP(int n, int s, int t): V(n), s(s), t(t), adj(V), lst(V + 1), gap(V), ex(V), h(V), cnt(V) {}

    void add_edge(int x, int to, T f, int is_dir) {
        assert(0 <= min(x, to) && max(x, to) < V);
        adj[x].emplace_back(to, adj[to].size(), f);
        adj[to].emplace_back(x, adj[x].size() - 1, is_dir ? 0 : f);
    }

    void clear() {
        for (int i = 0; i < V; ++i) {
            adj[i].clear();
            lst[i].clear();
            gap[i].clear();
        }
        mxh = work = 0;
        fill(h.begin(), h.end(), 0);
        fill(cnt.begin(), cnt.end(), 0);
        fill(ex.begin(), ex.end(), 0);
    }

    T calc(int heur_n = -1) {
        if (heur_n == -1) heur_n = V;
        fill(ex.begin(), ex.end(), 0);
        ex[s] = INF;
        globalRelabel();
        for (auto& e : adj[s]) push(s, e);
        for (; mxh >= 0; --mxh) {
            while (!lst[mxh].empty()) {
                int v = lst[mxh].back();
                lst[mxh].pop_back();
                discharge(v);
                if (work > 4 * heur_n) globalRelabel();
            }
        }
        return ex[t];
    }
};