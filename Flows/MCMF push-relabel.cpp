//Implementation of push-relabel algorithm
template<typename T_flow = int, typename T_cost = int>
class min_cost_max_flow {
    struct edge {
        T_cost c;
        T_flow f;
        int to, rev;
        edge(int to, T_cost c, T_flow f, int rev): c(c), f(f), to(to), rev(rev) {}
    };

    static constexpr T_cost INFCOST = numeric_limits<T_cost>::max() / 2;
    T_cost eps;
    int N, S, T;
    vector<vector<edge>> G;
    vector<vector<int>> hs;
    vector<size_t> isq, cur;
    vector<T_flow> ex;
    vector<T_cost> h;
    vector<int> co;

    void add_flow(edge& e, T_flow f) {
        edge& back = G[e.to][e.rev];
        if (!ex[e.to] && f) hs[h[e.to]].push_back(e.to);
        e.f -= f; ex[e.to] += f;
        back.f += f; ex[back.to] -= f;
    }

    T_flow max_flow() {
        ex.assign(N, 0);
        h.assign(N, 0); hs.resize(2 * N);
        co.assign(2 * N, 0); cur.assign(N, 0);
        h[S] = N;
        ex[T] = 1;
        co[0] = N - 1;
        for (auto& e : G[S]) add_flow(e, e.f);
        if (hs[0].size()) {
            for (int hi = 0; hi >= 0;) {
                int u = hs[hi].back();
                hs[hi].pop_back();
                while (ex[u] > 0) { // discharge u
                    if (cur[u] == G[u].size()) {
                        h[u] = 1e9;
                        for (size_t i = 0; i < G[u].size(); ++i) {
                            auto& e = G[u][i];
                            if (e.f && h[u] > h[e.to] + 1) {
                                h[u] = h[e.to] + 1, cur[u] = i;
                            }
                        }
                        if (++co[h[u]], !--co[hi] && hi < N) {
                            for (int i = 0; i < N; ++i) {
                                if (hi < h[i] && h[i] < N) {
                                    --co[h[i]];
                                    h[i] = N + 1;
                                }
                            }
                        }
                        hi = h[u];
                    } else if (G[u][cur[u]].f && h[u] == h[G[u][cur[u]].to] + 1) {
                        add_flow(G[u][cur[u]], min(ex[u], G[u][cur[u]].f));
                    } else {
                        ++cur[u];
                    }
                }
                while (hi >= 0 && hs[hi].empty()) --hi;
            }
        }
        return -ex[S];
    }

    void push(edge& e, T_flow amt) {
        if (e.f < amt) amt = e.f;
        e.f -= amt; ex[e.to] += amt;
        G[e.to][e.rev].f += amt; ex[G[e.to][e.rev].to] -= amt;
    }

    void relabel(int vertex) {
        T_cost newHeight = -INFCOST;
        for (size_t i = 0; i < G[vertex].size(); ++i) {
            edge const& e = G[vertex][i];
            if (e.f && newHeight < h[e.to] - e.c) {
                newHeight = h[e.to] - e.c;
                cur[vertex] = i;
            }
        }
        h[vertex] = newHeight - eps;
    }

public:
    min_cost_max_flow(int N, int S, int T): eps(0), N(N), S(S), T(T), G(N) {}

    //edge is directed
    void add_edge(int a, int b, T_flow cap, T_cost cost) {
        assert(cap >= 0);
        assert(0 <= min(a, b) && max(a, b) < N);
        if (a == b) {assert(cost >= 0); return;}
        cost *= N;
        eps = max(eps, abs(cost));
        G[a].emplace_back(b, cost, cap, G[b].size());
        G[b].emplace_back(a, -cost, 0, G[a].size() - 1);
    }

    static constexpr int scale = 1;
    pair<T_flow, T_cost> calc() {
        T_cost retCost = 0;
        for (int i = 0; i < N; ++i) {
            for (edge& e : G[i]) retCost += e.c * (e.f);
        }
        T_flow retFlow = max_flow();
        h.assign(N, 0); ex.assign(N, 0);
        isq.assign(N, 0); cur.assign(N, 0);
        queue<int> q;
        for (; eps; eps >>= scale) {
            fill(cur.begin(), cur.end(), 0);
            for (int i = 0; i < N; ++i) {
                for (auto& e : G[i]) {
                    if (h[i] + e.c - h[e.to] < 0 && e.f) push(e, e.f);
                }
            }
            for (int i = 0; i < N; ++i) {
                if (ex[i] > 0) {
                    q.push(i);
                    isq[i] = 1;
                }
            }
            while (!q.empty()) {
                int u = q.front(); q.pop();
                isq[u] = 0;
                while (ex[u] > 0) {
                    if (cur[u] == G[u].size()) relabel(u);
                    for (size_t& i = cur[u], max_i = G[u].size(); i < max_i; ++i) {
                        edge& e = G[u][i];
                        if (h[u] + e.c - h[e.to] < 0) {
                            push(e, ex[u]);
                            if (ex[e.to] > 0 && isq[e.to] == 0) {
                                q.push(e.to);
                                isq[e.to] = 1;
                            }
                            if (ex[u] == 0) break;
                        }
                    }
                }
            }
            if (eps > 1 && eps >> scale == 0) eps = 1 << scale;
        }
        for (int i = 0; i < N; ++i) {
            for (edge& e : G[i]) retCost -= e.c * e.f;
        }
        return {retFlow, retCost / 2 / N};
    }

    T_flow getFlow(const edge& e) {
        return G[e.to][e.rev].f;
    }
};
