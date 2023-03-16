//Implementation of push-relabel algorithm
template<typename flow_t = int, typename cost_t = int>
struct min_cost_max_flow {
    struct Edge {
        cost_t c;
        flow_t f;
        int to, rev;
        Edge(int to, cost_t c, flow_t f, int rev): c(c), f(f), to(to), rev(rev) {}
    };

private:
    static constexpr cost_t INFCOST = numeric_limits<cost_t>::max() / 2;
    cost_t eps;
    int N, S, T;
    vector<vector<Edge>> G;
    vector<vector<int>> hs;
    vector<size_t> isq, cur;
    vector<flow_t> ex;
    vector<cost_t> h;
    vector<int> co;

    void add_flow(Edge& e, flow_t f) {
        Edge& back = G[e.to][e.rev];
        if (!ex[e.to] && f) hs[h[e.to]].push_back(e.to);
        e.f -= f; ex[e.to] += f;
        back.f += f; ex[back.to] -= f;
    }

    flow_t max_flow() {
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

    void push(Edge& e, flow_t amt) {
        if (e.f < amt) amt = e.f;
        e.f -= amt; ex[e.to] += amt;
        G[e.to][e.rev].f += amt; ex[G[e.to][e.rev].to] -= amt;
    }

    void relabel(int vertex) {
        cost_t newHeight = -INFCOST;
        for (size_t i = 0; i < G[vertex].size(); ++i) {
            Edge const& e = G[vertex][i];
            if (e.f && newHeight < h[e.to] - e.c) {
                newHeight = h[e.to] - e.c;
                cur[vertex] = i;
            }
        }
        h[vertex] = newHeight - eps;
    }

public:
    min_cost_max_flow(int N, int S, int T): eps(0), N(N), S(S), T(T), G(N) {}

    //Edge is directed
    void add_edge(int a, int b, flow_t cap, cost_t cost) {
        assert(cap >= 0);
        assert(0 <= min(a, b) && max(a, b) < N);
        if (a == b) {assert(cost >= 0); return;}
        cost *= N;
        eps = max(eps, abs(cost));
        G[a].emplace_back(b, cost, cap, G[b].size());
        G[b].emplace_back(a, -cost, 0, G[a].size() - 1);
    }

    static constexpr int scale = 1;
    pair<flow_t, cost_t> calc() {
        cost_t retCost = 0;
        for (int i = 0; i < N; ++i) {
            for (Edge& e : G[i]) retCost += e.c * (e.f);
        }
        flow_t retFlow = max_flow();
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
                        Edge& e = G[u][i];
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
            for (Edge& e : G[i]) retCost -= e.c * e.f;
        }
        return {retFlow, retCost / 2 / N};
    }

    flow_t getFlow(const Edge& e) {
        return G[e.to][e.rev].f;
    }
};