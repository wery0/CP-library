template<typename Data>
struct edge {
    int to;
    Data data;

    edge() = default;
    edge(int to) : to(to) {}
    edge(int to, Data data) : to(to), data(data) {}

    bool operator<(const edge& other) const {return to < other.to;}
    bool operator==(const edge& other) const {return to == other.to && data == other.data;}
};

template<>
struct edge<void> {
    int to;

    edge() = default;
    edge(int to) : to(to) {}

    bool operator<(const edge& other) const {return to < other.to;}
    bool operator==(const edge& other) const {return to == other.to;}
};

template<typename Edge>
class tree {

    struct edge_range {
        __gnu_cxx::__normal_iterator<Edge*, vector<Edge>> first, last;
        auto begin() const {return first;}
        auto end() const {return last;}

        int size() const {return last - first;}
        Edge& operator[](const int v) {return *(first + v);}
    };

    struct graph {

        int V = -1, E;
        vector<Edge> store;
        vector<int> fir;
        vector<pair<int, Edge>> acc_edge;
        bool is_prepared = 0;

        graph() = default;
        graph(int V) : V(V) {}
        graph(int V, int E) : V(V) {if (E != -1) acc_edge.reserve(E);}

        void prepare() {if (V == -1) {int mxn = -1; for (const auto& [v, e] : acc_edge) mxn = max({mxn, v, e.to}); V = mxn + 1;} E = acc_edge.size(); store.resize(E); vector<int> deg_cnt(V + 1); for (const auto& p : acc_edge) ++deg_cnt[p.first + 1]; partial_sum(deg_cnt.begin(), deg_cnt.end(), deg_cnt.begin()); fir = deg_cnt; for (const auto& p : acc_edge) store[deg_cnt[p.first]++] = p.second; acc_edge.clear(); acc_edge.shrink_to_fit(); is_prepared = 1;}
        void add_edge(int x, Edge e, const bool is_dir) {if (V != -1) assert(0 <= x && x < V), assert(0 <= e.to && e.to < V); acc_edge.emplace_back(x, e); if (!is_dir) {swap(x, e.to); acc_edge.emplace_back(x, e);}}
        edge_range operator[](const int v) {assert(is_prepared); return {store.begin() + fir[v], store.begin() + fir[v + 1]};}
    };

    graph g;
    int V = -1;
    bool is_prepared = false;

    void ifs(int v, int p = -1) {
        static int T = 0; T = p == -1 ? 0 : T;
        tin[v] = T++;
        pr[v] = p;
        sz[v] = 1;
        dep[v] = p == -1 ? 0 : dep[p] + 1;
        if (dep[v] == 0) {
            jump[v] = v;
        } else if (dep[pr[v]] + dep[jump[jump[pr[v]]]] == dep[jump[pr[v]]] * 2) {
            jump[v] = jump[jump[pr[v]]];
            if (val.size() == V) {
                if (pr[v] != root) merge_val(jump_val[v], jump_val[pr[v]]);
                if (jump[pr[v]] != pr[v]) merge_val(jump_val[v], jump_val[jump[pr[v]]]);
            }
        } else {
            jump[v] = pr[v];
        }
        for (const auto& e : g[v]) {
            if (e.to == p) continue;
            //wdep[e.to] = wdep[v] + e.data;
            ifs(e.to, v);
            sz[v] += sz[e.to];
        }
        tout[v] = T;
    }

public:
    void prepare_and_root(int root_number) {
        assert(!is_prepared);
        g.prepare(); V = g.V;
        assert(g.E == (V - 1) * 2);
        pr.resize(V);
        jump.resize(V);
        if (val.size() == V) jump_val = val;
        dep.resize(V);
        sz.resize(V);
        tin.resize(V);
        tout.resize(V);
        //wdep.resize(V);
        is_prepared = true;
        reroot(root_number);
        assert(sz[root_number] == V && "Graph is not a tree!");
    }

    vector<int> pr;
    vector<int> jump;
    vector<int> dep;
    vector<int> sz;
    vector<int> tin, tout;
    int root = -1;
    //vector<int64_t> wdep;

    using U = int;
    vector<U> val;
    vector<U> jump_val;
    static constexpr auto merge_val = [](U& l, const U r) {l = min(l, r);};
    U NEUTRAL = numeric_limits<U>::max();
    U path_query(int x, int y, bool is_vertex_query = true) {
        assert(val.size() == V && jump_val.size() == V);
        // if (tin[x] > tin[y]) swap(x, y);
        U res = NEUTRAL;
        auto vertical_path_query = [&](int v, int k) {
            while (k) {
                int jump_len = dep[v] - dep[jump[v]];
                if (jump_len <= k) {
                    merge_val(res, jump_val[v]);
                    v = jump[v], k -= jump_len;
                } else {
                    merge_val(res, val[v]);
                    v = pr[v], --k;
                }
            }
        };
        int lc = calc_lca(x, y);
        vertical_path_query(x, dep[x] - dep[lc]);
        vertical_path_query(y, dep[y] - dep[lc]);
        if (is_vertex_query) merge_val(res, val[lc]);
        return res;
    }

    tree() = default;
    tree(int V): V(V) {assert(V > 0); g = graph(V, (V - 1) * 2);}

    void add_edge(int x, Edge e) {assert(!is_prepared); g.add_edge(x, e, 0);}
    void reroot(int new_root) {assert(is_prepared); if (root != new_root) ifs(root = new_root);}
    edge_range operator[](const int v) {assert(is_prepared); return g[v];}
    int size() const {return V;}

    bool is_descendant(int p, int v) {assert(is_prepared); return tin[p] <= tin[v] && tout[v] <= tout[p];}
    bool is_vertex_on_path(int v, int x, int y, int lc = -1) {if (lc == -1) lc = calc_lca(x, y); return is_descendant(lc, v) && (is_descendant(v, x) || is_descendant(v, y));}
    //int64_t calc_distance_weighted(int x, int y, int lc = -1) {if (lc == -1) lc = calc_lca(x, y); return wdep[x] + wdep[y] - wdep[lc] * 2;}
    int calc_distance_unweighted(int x, int y, int lc = -1) {if (lc == -1) lc = calc_lca(x, y); return dep[x] + dep[y] - dep[lc] * 2;}
    int calc_lca(int x, int y) {assert(is_prepared); if (dep[x] > dep[y]) swap(x, y); y = calc_kth_ancestor(y, dep[y] - dep[x]); if (x == y) return x; for (; pr[x] != pr[y];) {int u1 = jump[x], u2 = jump[y]; if (u1 == u2) x = pr[x], y = pr[y]; else x = u1, y = u2;} return pr[x];}
    int calc_kth_ancestor(int v, int k) {assert(is_prepared); assert(k <= dep[v]); for (; k;) {int u = jump[v], dfd = dep[v] - dep[u]; if (dfd <= k) v = u, k -= dfd; else v = pr[v], k--;} return v;}
    int calc_kth_vertex_on_path(int x, int y, int k, int lc = -1) {if (lc == -1) lc = calc_lca(x, y); int dst = dep[x] + dep[y] - dep[lc] * 2; if (k > dst) return -1; int d1 = dep[x] - dep[lc]; if (k <= d1) return calc_kth_ancestor(x, k); return calc_kth_ancestor(y, dst - k);}
    int calc_centroid() {assert(is_prepared); int v = root; int stv = v, p = -1; for (;;) {int mxsz = -1, h = -1; for (const auto& e : (*this)[v]) {if (e.to == p) continue; if (sz[e.to] > mxsz) mxsz = sz[e.to], h = e.to;} if (mxsz <= sz[stv] / 2) return v; p = v, v = h;} return -1;}
    int calc_diameter_unweighted() {assert(is_prepared); int v = max_element(dep.begin(), dep.end()) - dep.begin(); auto dfs = [&](auto&& dfs, int v, int p) -> int {int mxd = -1; for (const auto& e : g[v]) {if (e.to != p) {int tyt = dfs(dfs, e.to, v); mxd = max(mxd, tyt);}} return mxd + 1;}; return dfs(dfs, v, -1);}
    vector<int> calc_euler_traversal() {assert(is_prepared); vector<int> eul(V); int T = 0; auto dfs = [&](auto&& dfs, int v, int p) -> void {eul[T++] = v; for (const auto& e : g[v]) {if (e.to == p) continue; dfs(dfs, e.to, v);}}; dfs(dfs, root, -1); return eul;}
    vector<int> calc_path_between_vertexes(int x, int y) {assert(is_prepared); vector<int> fir, sec; while (!is_descendant(x, y)) fir.push_back(x), x = pr[x]; while (x != y) sec.push_back(y), y = pr[y]; fir.push_back(x); reverse(sec.begin(), sec.end()); for (int i : sec) fir.push_back(i); return fir;}
    vector<int> calc_foreach_furthest_vertex_unweighted() {assert(is_prepared); vector<int> deepest1(V), deepest2(V, -1); auto dfs = [&](auto dfs, int v, int p = -1) -> int {deepest1[v] = v; for (const auto& e : g[v]) {if (e.to == p) continue; int tyt = dfs(dfs, e.to, v); if (dep[tyt] >= dep[deepest1[v]]) {deepest2[v] = deepest1[v]; deepest1[v] = tyt;} else if (dep[tyt] > dep[deepest2[v]]) {deepest2[v] = tyt;}} return deepest1[v];}; dfs(dfs, root); vector<int> furthest(V); auto go = [&](auto go, int v, int u = -1, int dst_to_u = -1, int p = -1) -> void {furthest[v] = dst_to_u > dep[deepest1[v]] - dep[v] ? u : deepest1[v]; for (const auto& e : g[v]) {if (e.to == p) continue; int nwu = deepest1[e.to] == deepest1[v] ? deepest2[v] : deepest1[v]; int nwd = nwu == -1 ? -1 : dep[nwu] - dep[v]; if (nwu == -1 || dst_to_u > dep[nwu] - dep[v]) nwu = u, nwd = dst_to_u; go(go, e.to, nwu, nwd + 1, v);}}; go(go, root); return furthest;}
    template<typename D> vector<int> calc_foreach_furthest_vertex_weighted() {assert(is_prepared); vector<int> deepest1(V), deepest2(V, -1); vector<D> wdep(V); auto dfs = [&](auto dfs, int v, int p = -1) -> int {deepest1[v] = v; for (const auto& e : g[v]) {if (e.to == p) continue; wdep[e.to] = wdep[v] + e.data; int tyt = dfs(dfs, e.to, v); if (wdep[tyt] >= wdep[deepest1[v]]) {deepest2[v] = deepest1[v]; deepest1[v] = tyt;} else if (wdep[tyt] > wdep[deepest2[v]]) {deepest2[v] = tyt;}} return deepest1[v];}; dfs(dfs, root); vector<int> furthest(V); auto go = [&](auto go, int v, int u = -1, D dst_to_u = -1, int p = -1) -> void {furthest[v] = dst_to_u > wdep[deepest1[v]] - wdep[v] ? u : deepest1[v]; for (const auto& e : g[v]) {if (e.to == p) continue; int nwu = deepest1[e.to] == deepest1[v] ? deepest2[v] : deepest1[v]; D nwd = nwu == -1 ? -1 : wdep[nwu] - wdep[v]; if (nwu == -1 || dst_to_u > wdep[nwu] - wdep[v]) nwu = u, nwd = dst_to_u; go(go, e.to, nwu, nwd + e.data, v);}}; go(go, root); return furthest;}
    int64_t calc_sum_of_distances_between_all_pairs_of_vertexes_unweighted() {int64_t o = 0; auto dfs = [&](auto&& dfs, int v, int pr) -> void {for (const auto& e : (*this)[v]) {if (e.to == pr) continue; dfs(dfs, e.to, v); o += (int64_t)sz[e.to] * (V - sz[e.to]);}}; dfs(dfs, root, -1); return o;}
    template<typename D> D calc_sum_of_distances_between_all_pairs_of_vertexes_weighted() {D o = 0; auto dfs = [&](auto&& dfs, int v, int pr) -> void {for (const auto& e : (*this)[v]) {if (e.to == pr) continue; dfs(dfs, e.to, v); o += (D)e.data * sz[e.to] * (V - sz[e.to]);}}; dfs(dfs, root, -1); return o;}
    vector<int> calc_rooted_isomorphic_subtrees_without_data_guaranteed() {vector<int> ans(V); map<vector<int>, int> mp; int cnt = 1; vector<pair<int, int>> stq = {{root, 0}}; while (stq.size()) {pair<int, int> p = stq.back(); const int pr = stq.size() == 1 ? -1 : stq[stq.size() - 2].first; int v = p.first; if (p.second < (*this)[v].size()) {++stq.back().second; if ((*this)[v][p.second].to != pr) stq.emplace_back((*this)[v][p.second].to, 0);} else {vector<int> tyt; tyt.reserve((*this)[v].size()); for (const auto& e : (*this)[v]) {if (e.to != pr) tyt.push_back(ans[e.to]);} sort(tyt.begin(), tyt.end()); int& i = mp[tyt]; if (!i) i = cnt++; ans[v] = i - 1; stq.pop_back();}} return ans;}
    vector<int> calc_prufer_code() {assert(is_prepared && V > 1); vector<int> cnt(V), pr(V); auto dfs = [&](auto&& dfs, int v, int p) -> void {pr[v] = p; for (const auto& e : g[v]) {if (e.to != p) dfs(dfs, e.to, v);}}; dfs(dfs, V - 1, -1); for (int q = 0; q < V; ++q) cnt[q] = g[q].size(); vector<int> ans(V - 2); for (int q = 0, leaf = find(cnt.begin(), cnt.end(), 1) - cnt.begin(), ptr = leaf + 1; q < V - 2; ++q) {int p = pr[leaf]; ans[q] = p; --cnt[leaf]; if (--cnt[p] == 1 && p < ptr) {leaf = p;} else {for (; cnt[ptr] != 1;) ++ptr; leaf = ptr++;}} return ans;}
    pair<int, int> calc_path_intersection(int x1, int y1, int x2, int y2, int lc1 = -1, int lc2 = -1) {static constexpr pair<int, int> NO = {-1, -1}; if (lc1 == -1) lc1 = calc_lca(x1, y1); if (lc2 == -1) lc2 = calc_lca(x2, y2); if (!is_descendant(lc1, x2) && !is_descendant(lc1, y2)) return NO; if (!is_descendant(lc1, lc2)) swap(x1, x2), swap(y1, y2), swap(lc1, lc2); if (lc1 != lc2 && !is_vertex_on_path(lc2, x1, y1, lc1)) return NO; int v1 = calc_lca(x1, x2), v2 = calc_lca(x1, y2); if (v1 == lc1) v1 = calc_lca(y1, x2); if (v2 == lc1) v2 = calc_lca(y1, y2); if (tin[v1] > tin[v2]) swap(v1, v2); return {v1, v2};}
    template<typename T> vector<int> calc_maxcost_independent_set(vector<T> cost) {assert(cost.size() == V); vector<array<T, 2>> dp(V); auto dfs = [&](auto&& dfs, int v, int p) -> void {dp[v][1] = cost[v]; for (const auto& e : (*this)[v]) {if (e.to == p) continue; dfs(dfs, e.to, v); dp[v][1] += dp[e.to][0]; dp[v][0] += max(dp[e.to][0], dp[e.to][1]);}}; dfs(dfs, root, -1); vector<int> ans; auto dfs2 = [&](auto&& dfs2, int v, int p, int mode) -> void {if (mode == 1) ans.push_back(v); for (const auto& e : (*this)[v]) {if (e.to == p) continue; dfs2(dfs2, e.to, v, mode ? 0 : dp[e.to][1] > dp[e.to][0]);}}; dfs2(dfs2, root, -1, dp[root][1] > dp[root][0]); return ans;}
    template<typename D> pair<tree<edge<D>>, vector<int>> calc_compressed_tree(vector<int> m) {sort(m.begin(), m.end(), [&](int l, int r) {return tin[l] < tin[r];}); for (size_t i = m.size(); i > 1; --i) {m.push_back(calc_lca(m[i - 2], m[i - 1]));} sort(m.begin(), m.end(), [&](int l, int r) {return tin[l] < tin[r];}); m.erase(unique(m.begin(), m.end()), m.end()); vector<pair<int, int>> egs; egs.reserve(m.size()); for (vector<int> stq; int v : m) {while (stq.size() && !is_descendant(stq.back(), v)) stq.pop_back(); if (stq.size()) egs.emplace_back(stq.back(), v); stq.push_back(v);} sort(m.begin(), m.end()); tree<edge<D>> res(m.size()); for (auto [x, y] : egs) {int cx = lower_bound(m.begin(), m.end(), x) - m.begin(); int cy = lower_bound(m.begin(), m.end(), y) - m.begin(); res.add_edge(cx, {cy, dep[y] - dep[x]});} return {res, m};}
};
//Usage: tree<edge<T>> t(V), where T is the type of info you want to store on edges, T could be void.
//Before using tree, add all edges and then call t.prepare_and_root(root_number).
