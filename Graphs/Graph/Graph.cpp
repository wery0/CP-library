template<typename Data>
struct edge {
    int to;
    Data data;

    edge() = default;
    edge(int to) : to(to) {}
    edge(int to, Data data) : to(to), data(data) {}

    bool operator<(const edge &other) const {return to < other.to;}
    bool operator==(const edge &other) const {return to == other.to && data == other.data;}
};

template<>
struct edge<void> {
    int to;

    edge() = default;
    edge(int to) : to(to) {}

    bool operator<(const edge &other) const {return to < other.to;}
    bool operator==(const edge &other) const {return to == other.to;}
};

template<typename Edge>
struct graph {

    struct edge_range {
        __gnu_cxx::__normal_iterator<Edge*, vector<Edge>> first, last;
        auto begin() const {return first;}
        auto end() const {return last;}

        int size() const {return last - first;}
        bool empty() const {return first == last;}
        Edge& operator[](const int v) {return *(first + v);}
    };

    int V = -1, E;
    vec<Edge> store;
    vec<int> fir;
    vec<pair<int, Edge>> acc_edge;
    bool is_prepared = 0;

    graph() = default;
    graph(int V) : V(V) {}
    graph(int V, int E) : V(V) {if (E != -1) acc_edge.reserve(E);}

    void prepare() {
        if (V == -1) {
            int mxn = -1;
            for (const auto &[v, e] : acc_edge) {
                chmax(mxn, v);
                chmax(mxn, e.to);
            }
            V = mxn + 1;
        }
        E = acc_edge.size();
        store.resize(E);
        vec<int> deg_cnt(V + 1);
        for (const auto &p : acc_edge) ++deg_cnt[p.F + 1];
        partial_sum(all(deg_cnt), deg_cnt.begin());
        fir = deg_cnt;
        for (const auto &p : acc_edge) store[deg_cnt[p.F]++] = p.S;
        acc_edge.clear();
        acc_edge.shrink_to_fit();
        is_prepared = 1;
    }

    inline void add_edge(int x, Edge e, const bool is_dir) {
        if (V != -1) assert(0 <= x && x < V), assert(0 <= e.to && e.to < V);
        acc_edge.pb({x, e});
        if (!is_dir) {
            swap(x, e.to);
            acc_edge.pb({x, e});
        }
    }

    vec<int> get_nums_of_inv_edges() {
        assert(is_prepared);
        vec<int> ans(E, -1);
        vec<bool> us(V); umap<ll, vec<int>> mp(E);
        for (int v = 0; v < V; ++v) {
            for (int i = fir[v]; i < fir[v + 1]; ++i) {
                const auto& e = store[i]; ll hs = 1ll * v * V + e.to;
                ll ihs = 1ll * e.to * V + v;
                if (mp.count(ihs)) {
                    vec<int>& kek = mp[ihs];
                    int ips = kek.back(); kek.pop_back();
                    if (kek.empty()) mp.erase(ihs);
                    ans[i] = ips; ans[ips] = i;
                } else mp[hs].pb(i);
            }
        } assert(mp.empty()); assert(count(all(ans), -1) == 0); return ans;
    }

    edge_range operator[](const int v) {assert(is_prepared); return {store.begin() + fir[v], store.begin() + fir[v + 1]};}
    int size() const {return V;}
    int deg(const int v) const {assert(is_prepared); return fir[v + 1] - fir[v];}
    int count_connected_components(int min_component_size = 1) {assert(is_prepared); vec<bool> us(V); int cnt = 0; auto dfs = [&](auto && dfs, int v) -> void {us[v] = 1; ++cnt; for (const auto &h : (*this)[v]) if (!us[h.to]) dfs(dfs, h.to);}; int ans = 0; for (int q = 0; q < V; ++q, cnt = 0) if (!us[q]) dfs(dfs, q), ans += cnt >= min_component_size; return ans;}
    int count_weakly_connected_components(int min_component_size = 1) {assert(is_prepared); vector<int> pr(V), sz(V, 1); iota(all(pr), 0); auto find = [&](auto && find, int v) -> int {return pr[v] = v == pr[v] ? v : find(find, pr[v]);}; for (int v = 0; v < V; ++v) {for (const auto& e : store) {int pv = find(find, v), ph = find(find, e.to); if (pv != ph) {if (sz[pv] > sz[ph]) swap(pv, ph); sz[ph] += sz[pv]; pr[pv] = ph;}}} int ans = 0; for (int q = 0; q < V; ++q) ans += pr[q] == q && sz[q] >= min_component_size; return ans;}

    bool is_tree() {return E == V - 1 && is_vertex_connected();}
    bool is_vertex_connected() {return count_connected_components() == 1;}
    bool is_edge_connected() {return count_connected_components(2) <= 1;}
    bool is_bipartite() {assert(is_prepared); vec<int> col(V, -1); bool fl = 1; auto dfs = [&](auto && dfs, int v, int c) -> void {col[v] = c; for (const auto &h : (*this)[v]) {if (col[h.to] == -1) dfs(dfs, h.to, c ^ 1); else fl &= col[h.to] != c;}}; for (int q = 0; q < V; ++q) if (col[q] == -1) dfs(dfs, q, 0); return fl;}
    bool is_simple() {return !has_self_edges() && !has_multiple_edges();}
    bool is_functional() {assert(is_prepared); for (int q = 0; q < V; ++q) if (deg(q) > 1) return false; return true;}
    bool is_acyclic_dir() {assert(is_prepared); vec<int> state(V); auto dfs = [&](auto && dfs, int v) -> bool{state[v] = 1; for (const auto &h : (*this)[v]) {if (state[h.to] == 1) return false; if (state[h.to] == 0 && !dfs(dfs, h.to)) return false;} state[v] = 2; return true;}; for (int q = 0; q < V; ++q) {if (state[q] == 0 && !dfs(dfs, q)) return false;} return true;}
    bool is_acyclic_undir() {assert(is_prepared); vec<int> state(V); auto dfs = [&](auto && dfs, int v, int p = -1) -> bool{state[v] = 1; int cnt_par = 0; for (const auto &h : (*this)[v]) {if (h.to == p) {if (++cnt_par == 1) continue;} if (state[h.to] == 1) return false; if (state[h.to] == 0 && !dfs(dfs, h.to, v)) return false;} state[v] = 2; return true;}; for (int q = 0; q < V; ++q) if (state[q] == 0 && !dfs(dfs, q)) return false; return true;}
    bool has_self_edges() {assert(is_prepared); for (int v = 0; v < V; ++v) {for (const auto &h : (*this)[v]) {if (v == h.to) return true;}} return false;}
    bool has_multiple_edges() {assert(is_prepared); for (int v = 0; v < V; ++v) {uset<int> s(deg(v)); for (const auto &h : (*this)[v]) {if (s.count(h.to)) return true; s.insert(h.to);}} return false;}
    bool has_eulerian_path_dir() {if (count_weakly_connected_components(2) > 1) return 0; vector<int> in_deg(V); for (const auto& e : store) ++in_deg[e.to]; int bad = 0; for (int q = 0; q < V; ++q) {if (in_deg[q] != deg(q)) {if (abs(deg(q) - in_deg[q]) > 1) return 0; if (++bad > 2) return 0;}} return bad == 2;}
    bool has_eulerian_path_undir() {if (!is_edge_connected()) return 0; int bad = 0; for (int q = 0; q < V; ++q) {bad += deg(q) & 1; if (bad > 2) return 0;} return bad == 2;}
    bool has_eulerian_cycle_undir() {if (!is_edge_connected()) return 0; for (int q = 0; q < V; ++q) {if (deg(q) & 1) return 0;} return 1;}
    bool has_eulerian_cycle_dir() {if (count_weakly_connected_components(2) > 1) return 0; vector<int> in_deg(V); for (const auto& e : store) ++in_deg[e.to]; for (int q = 0; q < V; ++q) if (in_deg[q] != deg(q)) return 0; return 1;}

    vec<int> get_eulerian_cycle_dir_vertex_nums() {assert(has_eulerian_cycle_dir()); vec<int> ans; ans.reserve(E + 1); int fv = -1; for (int v = 0; v < V; ++v) if (deg(v)) {fv = v; break;} if (fv == -1) return {}; vec<int> stq = {fv}, ptr(V); while (stq.size()) {int v = stq.back(); if (ptr[v] == deg(v)) {ans.pb(v); stq.pop_back();} else stq.pb((*this)[v][ptr[v]++].to);} reverse(all(ans)); return ans;}
    vec<int> get_eulerian_cycle_undir_vertex_nums() {assert(has_eulerian_cycle_undir()); vec<int> ans; ans.reserve(E / 2 + 1); int fv = -1; for (int v = 0; v < V; ++v) if (deg(v)) {fv = v; break;} if (fv == -1) return {}; vec<int> stq = {fv}, ptr(V); vec<int> invn = get_nums_of_inv_edges(); for (int i = 0; i < store.size(); ++i) {invn[i] -= fir[store[i].to];} while (stq.size()) {int v = stq.back(); for (; ptr[v] < deg(v); ++ptr[v]) {int to = (*this)[v][ptr[v]].to; if (ptr[to] <= invn[fir[v] + ptr[v]]) break;} if (ptr[v] == deg(v)) {ans.pb(v); stq.pop_back();} else {stq.pb((*this)[v][ptr[v]++].to);}} reverse(all(ans)); return ans;}
    vec<int> get_any_dir_cycle_vertex_nums() {assert(!is_acyclic_dir()); vec<int> state(V), ans; auto dfs = [&](auto && dfs, int v) -> int{state[v] = 1; int ret = -1; for (const auto &h : (*this)[v]) {if (state[h.to] == 1) return h.to; int res = dfs(dfs, h.to); if (res == -1) continue; if (res == -2) {ret = -2; break;} ans.pb(h.to); if (v == res) ans.pb(v); return v != res ? res : -2;} state[v] = 2; return ret;}; for (int q = 0; q < V && ans.empty(); ++q) if (state[q] == 0) dfs(dfs, q); assert(!ans.empty()); reverse(all(ans)); return ans;}
    vec<int> get_any_dir_cycle_edge_nums() {assert(!is_acyclic_dir()); vec<int> state(V), ans; auto dfs = [&](auto && dfs, int v) -> int{state[v] = 1; int ret = -1; for (const auto &h : (*this)[v]) {if (state[h.to] == 1) {ans.pb(h.data); return h.to;} int res = dfs(dfs, h.to); if (res == -1) continue; if (res == -2) {ret = -2; break;} ans.pb(h.data); return v != res ? res : -2;} state[v] = 2; return ret;}; for (int q = 0; q < V && ans.empty(); ++q) if (state[q] == 0) dfs(dfs, q); assert(!ans.empty()); reverse(all(ans)); return ans;}
    vec<int> get_topsort() {assert(is_acyclic_dir()); vec<bool> us(V); vec<int> ans; ans.reserve(V); auto dfs = [&](auto && dfs, int v) -> void {us[v] = 1; for (const auto& e : (*this)[v]) {if (!us[e.to]) dfs(dfs, e.to);} ans.pb(v);}; for (int q = 0; q < V; ++q) {if (!us[q]) dfs(dfs, q);} reverse(all(ans)); return ans;}
    vec<int> get_cutpoints() {assert(is_prepared); vec<bool> us(V); vec<int> is_cp(V), tin(V), fup(V); auto dfs = [&](auto && dfs, int v, int p = -1) -> void {static int T = 0; us[v] = 1; tin[v] = fup[v] = T++; int chd = 0; for (const auto &h : (*this)[v]) {if (h.to == p) continue; if (!us[h.to]) {++chd; dfs(dfs, h.to, v); chmin(fup[v], fup[h.to]); is_cp[v] |= p == -1 ? chd > 1 : fup[h.to] >= tin[v];} else chmin(fup[v], tin[h.to]);}}; for (int q = 0; q < V; ++q) if (!us[q]) dfs(dfs, q); vec<int> ans; for (int q = 0; q < V; ++q) if (is_cp[q]) ans.pb(q); return ans;}
    vec<int> get_bridges_nums() {assert(is_prepared); vec<bool> is_bridge(E), us(V); vec<int> tin(V), fup(V); auto dfs = [&](auto dfs, int v, int p = -1) -> int {static int T = 0; us[v] = 1; tin[v] = fup[v] = T++; int cnt_par = 0, ie = -1; for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& h = store[i]; if (h.to == p) {if (++cnt_par > 1) {chmin(fup[v], tin[h.to]);} else ie = i;} else if (!us[h.to]) {int inv_edge = dfs(dfs, h.to, v); chmin(fup[v], fup[h.to]); is_bridge[i] = is_bridge[inv_edge] = fup[h.to] > tin[v];} else chmin(fup[v], tin[h.to]);} return ie;}; for (int q = 0; q < V; ++q) if (!us[q]) dfs(dfs, q); int cnt_bridges = count(all(is_bridge), true) / 2; vec<int> ans(cnt_bridges); for (int q = 0, j = 0; q < V; ++q) {for (int i = fir[q]; i < fir[q + 1]; ++i) {const auto &h = store[i]; if (is_bridge[i] && q < h.to) ans[j++] = h.data;}} return ans;}
    vec<vec<int>> get_connected_components_undir() {assert(is_prepared); vec<vec<int>> ans; vec<bool> us(V); auto dfs = [&](auto && dfs, int v) -> void {us[v] = 1; ans.back().pb(v); for (const auto& e : (*this)[v]) {if (us[e.to]) continue; dfs(dfs, e.to);}}; for (int q = 0; q < V; ++q) {if (us[q]) continue; ans.pb({}); dfs(dfs, q);} return ans;}
    vec<vec<int>> get_vertex_biconnected_components() {assert(is_prepared); vec<bool> us(V); vec<int> tin(V), fup(V), stq; vec<vec<int>> ans; ans.reserve(V); stq.reserve(V); auto dfs = [&](auto && dfs, int v, int p = -1) -> void {static int T = 0; stq.pb(v); us[v] = 1; tin[v] = fup[v] = T++; int chd = 0; for (const auto &h : (*this)[v]) {if (h.to == p) continue; if (!us[h.to]) {++chd; int stop_sz = stq.size(); dfs(dfs, h.to, v); chmin(fup[v], fup[h.to]); if ((p == -1 && chd > 1) || (p != -1 && fup[h.to] >= tin[v])) {ans.pb({v}); ans.back().reserve(stq.size() - stop_sz + 1); for (; stq.size() > stop_sz;) {ans.back().pb(stq.back()); stq.pop_back();}}} else chmin(fup[v], tin[h.to]);}}; for (int q = 0; q < V; ++q) {if (!us[q]) {dfs(dfs, q); ans.pb(stq); stq.clear();}} return ans;}
    vec<vec<int>> get_edge_biconnected_components() {assert(is_prepared); vec<bool> is_bridge(E), us(V); vec<int> tin(V), fup(V); auto dfs = [&](auto dfs, int v, int p = -1) -> int {static int T = 0; us[v] = 1; tin[v] = fup[v] = T++; int cnt_par = 0, ie = -1; for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& h = store[i]; if (h.to == p) {if (++cnt_par > 1) {chmin(fup[v], tin[h.to]);} else ie = i;} else if (!us[h.to]) {int inv_edge = dfs(dfs, h.to, v); chmin(fup[v], fup[h.to]); is_bridge[i] = is_bridge[inv_edge] = fup[h.to] > tin[v];} else chmin(fup[v], tin[h.to]);} return ie;}; int cnt_cmp = 0; for (int q = 0; q < V; ++q) if (!us[q]) dfs(dfs, q), ++cnt_cmp; int cnt_bridges = count(all(is_bridge), true) / 2; int cnt_tec_cmp = cnt_cmp + cnt_bridges; vec<vec<int>> ans(cnt_tec_cmp); fill(all(us), false); auto go = [&](auto && go, int v, int nc) -> void {us[v] = 1; assert(nc < cnt_tec_cmp); ans[nc].pb(v); for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& h = store[i]; if (!us[h.to] && !is_bridge[i]) go(go, h.to, nc);}}; for (int q = 0, nc = 0; q < V; ++q) if (!us[q]) go(go, q, nc++); return ans;}

    //dst[v] = -1, if no path from cluster
    vec<int> BFS_from_cluster(vec<int> cluster) {assert(is_prepared); vec<int> dst(V, -1); for (int i : cluster) dst[i] = 0; deque<int> dq(all(cluster)); while (dq.size()) {int v = dq.front(); dq.pop_front(); for (const auto &e : (*this)[v]) {if (dst[e.to] == -1) {dst[e.to] = dst[v] + 1; dq.pb(e.to);}}} return dst;}
    vec<int> BFS_from_cluster_on_inverse_graph(vec<int> cluster) {assert(is_prepared); vec<int> dst(V, -1); for (int i : cluster) dst[i] = 0; vec<int> s; for (int q = 0; q < V; ++q) if (dst[q] != 0) s.pb(q); deque<int> dq(all(cluster)); vec<bool> tyt(V); while (dq.size()) {int v = dq.front(); dq.pop_front(); for (const auto &e : (*this)[v]) tyt[e.to] = 1; for (int q = 0; q < s.size(); ++q) {const int h = s[q]; if (!tyt[h]) {dst[h] = dst[v] + 1; dq.pb(h); if (q + 1 < s.size()) swap(s[q], s.back()); s.pop_back(); --q;}} for (const auto &e : (*this)[v]) tyt[e.to] = 0;} return dst;}

    vec<int> MIS_greedy() {vec<int> dg(V); set<pii> s; for (int q = 0; q < V; ++q) {dg[q] = deg(q); s.insert({dg[q], q});} vec<int> ans; while (s.size()) {auto [d, v] = *s.begin(); s.erase(s.begin()); ans.pb(v); dg[v] = -1; for (const auto &e : (*this)[v]) {if (dg[e.to] != -1) {s.erase({dg[e.to], e.to}); dg[e.to] = -2;}} for (const auto &e : (*this)[v]) {if (dg[e.to] == -2) {for (const auto &ee : (*this)[e.to]) {if (dg[ee.to] > 0) {s.erase({dg[ee.to], ee.to}); --dg[ee.to]; s.insert({dg[ee.to], ee.to});}} dg[e.to] = -1;}}} return ans;}
};