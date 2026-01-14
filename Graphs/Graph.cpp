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
class graph {

    struct edge_range {
        __gnu_cxx::__normal_iterator<Edge*, vector<Edge>> first, last;
        auto begin() const {return first;}
        auto end() const {return last;}

        int size() const {return last - first;}
        bool empty() const {return first == last;}
        Edge& operator[](int v) {return *(first + v);}
        const Edge& operator[](int v) const {return *(first + v);}
    };

    int V = -1, E;
    vector<Edge> store;
    vector<int> fir;
    vector<pair<int, Edge>> acc_edge;
    bool is_prepared = 0;

    int deg(const int v) const {assert(is_prepared); return fir[v + 1] - fir[v];}
    vector<int> get_nums_of_inverse_edges() {assert(is_prepared); vector<int> ans(E, -1); vector<bool> us(V); unordered_map<int64_t, vector<int>> mp(E); for (int v = 0; v < V; ++v) {for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& e = store[i]; int64_t hs = (int64_t)v * V + e.to; int64_t ihs = (int64_t)e.to * V + v; if (mp.count(ihs)) {vector<int>& kek = mp[ihs]; int ips = kek.back(); kek.pop_back(); if (kek.empty()) mp.erase(ihs); ans[i] = ips; ans[ips] = i;} else mp[hs].push_back(i);}} assert(mp.empty()); assert(count(ans.begin(), ans.end(), -1) == 0); return ans;}
    vector<int> get_any_cycle_vertex_nums(bool is_dir) {assert(!(is_dir ? is_acyclic_directed() : is_acyclic_undirected())); vector<int> state(V), ans; function<int(int, int)> dfs = [&](int v, int p) -> int {state[v] = 1; int ret = -1, cntp = 0; for (const auto& h : (*this)[v]) {if (h.to == p && ++cntp == 1) continue; if (state[h.to] == 1) {if (h.to == v) {ans = {v}; return -2;} return h.to;} int res = dfs(h.to, is_dir ? -1 : v); if (res == -1) continue; if (res == -2) {ret = -2; break;} ans.push_back(h.to); if (v == res) ans.push_back(v); return v != res ? res : -2;} state[v] = 2; return ret;}; for (size_t v = 0; v < V && ans.empty(); ++v) if (state[v] == 0) dfs(v, -1); assert(!ans.empty()); reverse(ans.begin(), ans.end()); return ans;}
    vector<int> get_any_odd_cycle_vertex_nums(bool is_dir) {assert(!(is_dir ? is_acyclic_directed() : is_acyclic_undirected())); vector<int> state(V), col(V), ans; function<int(int, int, int)> dfs = [&](int v, int c, int p) -> int {state[v] = 1; col[v] = c; int ret = -1, cntp = 0; for (const auto& h : (*this)[v]) {if (h.to == p && ++cntp == 1) continue; if (state[h.to] == 1) {if (col[h.to] != col[v]) continue; if (h.to == v) {ans = {v}; return -2;} return h.to;} int res = dfs(h.to, c ^ 1, is_dir ? -1 : v); if (res == -1) continue; if (res == -2) {ret = -2; break;} ans.push_back(h.to); if (v == res) ans.push_back(v); return v != res ? res : -2;} state[v] = 2; return ret;}; for (size_t v = 0; v < V && ans.empty(); ++v) if (state[v] == 0) dfs(v, 0, -1); assert(!ans.empty()); reverse(ans.begin(), ans.end()); return ans;}
    vector<int> get_any_cycle_edge_nums(bool is_dir) {assert(!(is_dir ? is_acyclic_directed() : is_acyclic_undirected())); vector<int> state(V), ans; function<int(int, int)> dfs = [&](int v, int p) -> int {state[v] = 1; int ret = -1, cntp = 0; for (const auto& h : (*this)[v]) {if (h.to == p && ++cntp == 1) continue; if (state[h.to] == 1) {if (h.to == v) {ans = {h.data}; return -2;} ans.push_back(h.data); return h.to;} int res = dfs(h.to, is_dir ? -1 : v); if (res == -1) continue; if (res == -2) {ret = -2; break;} ans.push_back(h.data); return v != res ? res : -2;} state[v] = 2; return ret;}; for (size_t v = 0; v < V && ans.empty(); ++v) if (state[v] == 0) dfs(v, -1); assert(!ans.empty()); reverse(ans.begin(), ans.end()); return ans;}

public:
    graph() = default;
    graph(int V) : V(V) {}
    graph(int V, int E) : V(V) {if (E != -1) acc_edge.reserve(E);}

    void prepare() {if (V == -1) {int mxn = -1; for (const auto& [v, e] : acc_edge) mxn = max({mxn, v, e.to}); V = mxn + 1;} E = acc_edge.size(); store.resize(E); vector<int> deg_cnt(V + 1); for (const auto& p : acc_edge) ++deg_cnt[p.first + 1]; partial_sum(deg_cnt.begin(), deg_cnt.end(), deg_cnt.begin()); fir = deg_cnt; for (const auto& [x, y] : acc_edge) store[deg_cnt[x]++] = y; acc_edge.clear(); acc_edge.shrink_to_fit(); is_prepared = 1;}
    void add_edge(int x, Edge e, bool is_directed) {if (V != -1) assert(0 <= x && x < V), assert(is_directed || (0 <= e.to && e.to < V)); acc_edge.emplace_back(x, e); if (!is_directed) {swap(x, e.to); acc_edge.emplace_back(x, e);}}
    void clear() {E = 0; store.clear(); fir.clear(); acc_edge.clear(); is_prepared = 0;}

    edge_range operator[](int v) {assert(is_prepared); return {store.begin() + fir[v], store.begin() + fir[v + 1]};}
    graph<edge<void>> get_complement_graph() {assert(is_simple()); graph<edge<void>> res(V, V * (V - 1) - E); for (int v = 0; v < V; ++v) {int l = fir[v], r = fir[v + 1]; sort(store.begin() + l, store.begin() + r); for (int h = 0; h < v; ++h) if (l == r || h < store[l].to) res.add_edge(v, h, 1); else ++l; for (int h = v + 1; h < V; ++h) if (l == r || h < store[l].to) res.add_edge(v, h, 1); else ++l;} res.prepare(); return res;}

    int size() const {return V;}
    int count_connected_components(int min_component_size = 1, bool count_single_vertex_components_with_self_edges = false) {assert(is_prepared); vector<char> us(V), has_self_edge(V); int cnt = 0; auto dfs = [&](auto&& dfs, int v) -> void {us[v] = 1; ++cnt; for (const auto& h : (*this)[v]) {if (v == h.to) has_self_edge[v] = 1; if (!us[h.to]) dfs(dfs, h.to);}}; int ans = 0; for (int v = 0; v < V; ++v, cnt = 0)if (!us[v]) dfs(dfs, v), ans += cnt >= min_component_size || count_single_vertex_components_with_self_edges && cnt == 1 && has_self_edge[v]; return ans;}
    int count_weakly_connected_components(int min_component_size = 1, bool count_single_vertex_components_with_self_edges = false) {assert(is_prepared); vector<int> pr(V), sz(V, 1); vector<char> has_self_edge(V); iota(pr.begin(), pr.end(), 0); auto find = [&](auto&& find, int v) -> int {return pr[v] = v == pr[v] ? v : find(find, pr[v]);}; for (int v = 0; v < V; ++v) {for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& e = store[i]; if (v == e.to) has_self_edge[v] = 1; int pv = find(find, v), ph = find(find, e.to); if (pv != ph) {if (sz[pv] > sz[ph]) swap(pv, ph); sz[ph] += sz[pv]; pr[pv] = ph;}}} int ans = 0; for (int v = 0; v < V; ++v) ans += pr[v] == v && (sz[v] >= min_component_size || count_single_vertex_components_with_self_edges && sz[v] == 1 && has_self_edge[v]); return ans;}

    bool is_tree() {return E == V - 1 && is_vertex_connected();}
    bool is_vertex_connected() {return V == 0 || count_connected_components() == 1;}
    bool is_edge_connected() {return count_connected_components(2, true) <= 1;}
    bool is_weakly_edge_connected() {return count_weakly_connected_components(2, true) <= 1;}
    bool is_bipartite() {assert(is_prepared); vector<int> col(V, -1); bool fl = 1; auto dfs = [&](auto&& dfs, int v, int c) -> void {col[v] = c; for (const auto& h : (*this)[v]) {if (col[h.to] == -1) dfs(dfs, h.to, c ^ 1); else fl &= col[h.to] != c;}}; for (int q = 0; q < V; ++q) if (col[q] == -1) dfs(dfs, q, 0); return fl;}
    bool is_simple() {return !has_self_edges() && !has_multiple_edges();}
    bool is_functional() {assert(is_prepared); for (int v = 0; v < V; ++v) if (deg(v) > 1) return false; return true;}
    bool is_acyclic_directed() {assert(is_prepared); vector<int> state(V); auto dfs = [&](auto&& dfs, int v) -> bool {state[v] = 1; for (const auto& h : (*this)[v]) {if (state[h.to] == 1) return false; if (state[h.to] == 0 && !dfs(dfs, h.to)) return false;} state[v] = 2; return true;}; for (int q = 0; q < V; ++q) {if (state[q] == 0 && !dfs(dfs, q)) return false;} return true;}
    bool is_acyclic_undirected() {assert(is_prepared); vector<int> state(V); auto dfs = [&](auto&& dfs, int v, int p = -1) -> bool {state[v] = 1; int cnt_par = 0; for (const auto& h : (*this)[v]) {if (h.to == p) {if (++cnt_par == 1) continue;} if (state[h.to] == 1) return false; if (state[h.to] == 0 && !dfs(dfs, h.to, v)) return false;} state[v] = 2; return true;}; for (int q = 0; q < V; ++q) if (state[q] == 0 && !dfs(dfs, q)) return false; return true;}
    bool has_self_edges() {assert(is_prepared); for (int v = 0; v < V; ++v) {for (const auto& h : (*this)[v]) {if (v == h.to) return true;}} return false;}
    bool has_multiple_edges() {assert(is_prepared); for (int v = 0; v < V; ++v) {unordered_set<int> s(deg(v)); for (const auto& h : (*this)[v]) {if (s.count(h.to)) return true; s.insert(h.to);}} return false;}
    bool has_eulerian_path_directed() {if (E == 0) return 1; if (!is_weakly_edge_connected()) return 0; vector<int> in_deg(V); for (const auto& e : store) ++in_deg[e.to]; int bad = 0; for (int v = 0; v < V; ++v) {if (in_deg[v] != deg(v)) {if (abs(deg(v) - in_deg[v]) > 1) return 0; if (++bad > 2) return 0;}} return bad == 0 || bad == 2;}
    bool has_eulerian_path_undirected() {if (E == 0) return 1; if (!is_edge_connected()) return 0; int bad = 0; for (int v = 0; v < V; ++v) {bad += deg(v) & 1; if (bad > 2) return 0;} return bad == 0 || bad == 2;}
    bool has_eulerian_cycle_directed() {if (E == 0) return 1; if (!is_weakly_edge_connected()) return 0; vector<int> in_deg(V); for (const auto& e : store) ++in_deg[e.to]; for (int v = 0; v < V; ++v) if (in_deg[v] != deg(v)) return 0; return 1;}
    bool has_eulerian_cycle_undirected() {if (E == 0) return 1; if (!is_edge_connected()) return 0; for (int v = 0; v < V; ++v) if (deg(v) & 1) return 0; return 1;}

    vector<int> get_any_cycle_as_vertex_nums(bool is_directed) {return get_any_cycle_vertex_nums(is_directed);}
    vector<int> get_any_cycle_as_edge_nums(bool is_directed) {return get_any_cycle_edge_nums(is_directed);}
    vector<int> get_any_odd_cycle_as_vertex_nums(bool is_directed) {return get_any_odd_cycle_vertex_nums(is_directed);}
    vector<int> get_eulerian_path_directed_as_vertex_nums() {assert(has_eulerian_path_directed()); vector<int> ans, in_deg(V); ans.reserve(E + 1); for (const auto& e : store) ++in_deg[e.to]; int fv = -1; for (int v = 0; v < V; ++v)if (in_deg[v] < deg(v)) {fv = v; break;} if (fv == -1)for (int v = 0; v < V; ++v)if (deg(v)) {fv = v; break;} if (fv == -1) return {}; vector<int> stq = {fv}, ptr(V); while (stq.size()) {int v = stq.back(); if (ptr[v] == deg(v)) {ans.push_back(v); stq.pop_back();} else {stq.push_back((*this)[v][ptr[v]++].to);}} reverse(ans.begin(), ans.end()); return ans;}
    vector<int> get_eulerian_path_undirected_as_vertex_nums() {assert(has_eulerian_path_undirected()); vector<int> ans; ans.reserve(E / 2 + 1); int fv = -1; for (int v = 0; v < V; ++v)if (deg(v) & 1) {fv = v; break;} if (fv == -1)for (int v = 0; v < V; ++v)if (deg(v)) {fv = v; break;} if (fv == -1) return {}; vector<int> stq = {fv}, ptr(V), invn = get_nums_of_inverse_edges(); for (int i = 0; i < store.size(); ++i) invn[i] -= fir[store[i].to]; while (stq.size()) {int v = stq.back(); for (; ptr[v] < deg(v); ++ptr[v]) {int to = (*this)[v][ptr[v]].to; if (ptr[to] <= invn[fir[v] + ptr[v]]) break;} if (ptr[v] == deg(v)) {ans.push_back(v); stq.pop_back();} else {stq.push_back((*this)[v][ptr[v]++].to);}} reverse(ans.begin(), ans.end()); return ans;}
    vector<int> get_topsort() {assert(is_acyclic_directed()); vector<bool> us(V); vector<int> ans; ans.reserve(V); auto dfs = [&](auto&& dfs, int v) -> void {us[v] = 1; for (const auto& e : (*this)[v]) {if (!us[e.to]) dfs(dfs, e.to);} ans.push_back(v);}; for (int q = 0; q < V; ++q) {if (!us[q]) dfs(dfs, q);} reverse(ans.begin(), ans.end()); return ans;}
    vector<int> get_cutpoints() {auto cnt = calc_connected_components_after_vertex_removal(); vector<int> ans; for (int v = 0; v < V; ++v) if (cnt[v] > 1) ans.push_back(v); return ans;}
    vector<int> get_bridges_as_edge_nums() {assert(is_prepared); vector<bool> is_bridge(E), us(V); vector<int> tin(V), fup(V); auto dfs = [&](auto dfs, int v, int p = -1) -> int {static int T = 0; us[v] = 1; tin[v] = fup[v] = T++; int cnt_par = 0, ie = -1; for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& h = store[i]; if (h.to == p) {if (++cnt_par > 1) {fup[v] = min(fup[v], tin[h.to]);} else ie = i;} else if (!us[h.to]) {int inv_edge = dfs(dfs, h.to, v); fup[v] = min(fup[v], fup[h.to]); is_bridge[i] = is_bridge[inv_edge] = fup[h.to] > tin[v];} else fup[v] = min(fup[v], tin[h.to]);} return ie;}; for (int q = 0; q < V; ++q) if (!us[q]) dfs(dfs, q); int cnt_bridges = count(is_bridge.begin(), is_bridge.end(), true) / 2; vector<int> ans(cnt_bridges); for (int q = 0, j = 0; q < V; ++q) {for (int i = fir[q]; i < fir[q + 1]; ++i) {const auto& h = store[i]; if (is_bridge[i] && q < h.to) ans[j++] = h.data;}} return ans;}
    vector<int> get_bipartite_coloring() {assert(is_prepared); vector<int> col(V, -1); bool fl = 1; auto dfs = [&](auto&& dfs, int v, int c) -> void {col[v] = c; for (const auto& h : (*this)[v]) {if (col[h.to] == -1) dfs(dfs, h.to, c ^ 1); else fl &= col[h.to] != c;}}; for (int v = 0, p = 1; v < V && fl; ++v) if (col[v] == -1) dfs(dfs, v, p ^= 1); assert(fl && "Graph is not bipartite"); return col;}
    vector<vector<int>> get_connected_components_undirected() {assert(is_prepared); vector<vector<int>> ans; vector<bool> us(V); auto dfs = [&](auto&& dfs, int v) -> void {us[v] = 1; ans.back().push_back(v); for (const auto& e : (*this)[v]) {if (us[e.to]) continue; dfs(dfs, e.to);}}; for (int q = 0; q < V; ++q) {if (us[q]) continue; ans.push_back({}); dfs(dfs, q);} return ans;}
    vector<vector<int>> get_vertex_biconnected_components() {assert(is_prepared); vector<bool> us(V); vector<int> tin(V), fup(V), stq; vector<vector<int>> ans; ans.reserve(V); stq.reserve(V); auto dfs = [&](auto&& dfs, int v, int p = -1) -> void {static int T = 0; stq.push_back(v); us[v] = 1; tin[v] = fup[v] = T++; int chd = 0; for (const auto& h : (*this)[v]) {if (h.to == p) continue; if (!us[h.to]) {++chd; int stop_sz = stq.size(); dfs(dfs, h.to, v); fup[v] = min(fup[v], fup[h.to]); if ((p == -1 && chd > 1) || (p != -1 && fup[h.to] >= tin[v])) {ans.push_back({v}); ans.back().reserve(stq.size() - stop_sz + 1); for (; stq.size() > stop_sz;) {ans.back().push_back(stq.back()); stq.pop_back();}}} else fup[v] = min(fup[v], tin[h.to]);}}; for (int q = 0; q < V; ++q) {if (!us[q]) {dfs(dfs, q); ans.push_back(stq); stq.clear();}} return ans;}
    vector<vector<int>> get_edge_biconnected_components() {assert(is_prepared); vector<bool> is_bridge(E), us(V); vector<int> tin(V), fup(V); auto dfs = [&](auto dfs, int v, int p = -1) -> int {static int T = 0; us[v] = 1; tin[v] = fup[v] = T++; int cnt_par = 0, ie = -1; for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& h = store[i]; if (h.to == p) {if (++cnt_par > 1) {fup[v] = min(fup[v], tin[h.to]);} else ie = i;} else if (!us[h.to]) {int inv_edge = dfs(dfs, h.to, v); fup[v] = min(fup[v], fup[h.to]); is_bridge[i] = is_bridge[inv_edge] = fup[h.to] > tin[v];} else fup[v] = min(fup[v], tin[h.to]);} return ie;}; int cnt_cmp = 0; for (int q = 0; q < V; ++q) if (!us[q]) dfs(dfs, q), ++cnt_cmp; int cnt_bridges = count(is_bridge.begin(), is_bridge.end(), true) / 2; int cnt_tec_cmp = cnt_cmp + cnt_bridges; vector<vector<int>> ans(cnt_tec_cmp); fill(us.begin(), us.end(), false); auto go = [&](auto&& go, int v, int nc) -> void {us[v] = 1; assert(nc < cnt_tec_cmp); ans[nc].push_back(v); for (int i = fir[v]; i < fir[v + 1]; ++i) {const auto& h = store[i]; if (!us[h.to] && !is_bridge[i]) go(go, h.to, nc);}}; for (int q = 0, nc = 0; q < V; ++q) if (!us[q]) go(go, q, nc++); return ans;}
    vector<int> calc_connected_components_after_vertex_removal() {assert(is_prepared); vector<bool> us(V); vector<int> ans(V, V > 1), tin(V), fup(V); auto dfs = [&](auto&& dfs, int v, int p = -1) -> void {static int T = 0; us[v] = 1; tin[v] = fup[v] = T++; int chd = 0; for (const auto& h : (*this)[v]) {if (h.to == p) continue; if (!us[h.to]) {++chd; dfs(dfs, h.to, v); fup[v] = min(fup[v], fup[h.to]); ans[v] += p == -1 ? chd > 1 : fup[h.to] >= tin[v];} else fup[v] = min(fup[v], tin[h.to]);}}; for (int v = 0; v < V; ++v) if (!us[v]) dfs(dfs, v); return ans;}
    pair<int, vector<int>> calc_scc_coloring() {assert(is_prepared); vector<int> color(V); vector<int> tin(V, -1), fup(V); stack<int> stq; vector<char> in_stack(V); vector<pair<int, int>> state; state.reserve(V); int ncolor = 0, T = 0; auto chmin = [](int& a, int b) {a = b < a ? b : a;}; for (int st = 0; st < V; ++st) {if (tin[st] == -1) {state = {{st, 0}}; tin[st] = fup[st] = T++; stq.push(st); in_stack[st] = 1; while (!state.empty()) {auto& [v, i] = state.back(); const auto& l = (*this)[v]; if (i < l.size()) {while (i < l.size() && tin[l[i].to] != -1) {if (in_stack[l[i].to]) chmin(fup[v], tin[l[i].to]); ++i;} if (i < l.size()) {int u = l[i].to; tin[u] = fup[u] = T++; stq.push(u); in_stack[u] = 1; state.emplace_back(u, 0); ++i;}} else {if (fup[v] == tin[v]) {while (true) {int u = stq.top(); stq.pop(); in_stack[u] = 0; color[u] = ncolor; if (u == v) break;}++ncolor;} state.pop_back(); if (state.size()) chmin(fup[state.back().first], fup[v]);}}}} for (auto& c : color) c = ncolor - 1 - c; return {ncolor, color};}
    tuple<int, vector<int>, graph<edge<void>>> calc_scc_coloring_and_condensation() {auto [ncolor, color] = calc_scc_coloring(); vector<pair<int, int>> egs; for (int v = 0; v < V; ++v) {for (const auto& e : (*this)[v]) {if (color[v] != color[e.to]) egs.emplace_back(color[v], color[e.to]);}} sort(egs.begin(), egs.end()); egs.erase(unique(egs.begin(), egs.end()), egs.end()); graph<edge<void>> g(ncolor, egs.size()); for (auto [x, y] : egs) g.add_edge(x, y, 1); g.prepare(); return {ncolor, color, g};}

    //Returns {dst, prv}
    //dst - distances from cluster
    //prv - previous nodes on shortest path from cluster
    //dst[v] = -1 if no path from cluster to v
    //prv[v] = -1  if no path from cluster to v or v is in cluster
    pair<vector<int>, vector<int>> BFS_from_cluster(vector<int> cluster) {assert(is_prepared); vector<int> dst(V, -1), pr(V, -1); for (int i : cluster) dst[i] = 0; deque<int> dq(cluster.begin(), cluster.end()); while (dq.size()) {int v = dq.front(); dq.pop_front(); for (const auto& e : (*this)[v]) {if (dst[e.to] == -1) {dst[e.to] = dst[v] + 1; pr[e.to] = v; dq.push_back(e.to);}}} return {dst, pr};}
    pair<vector<int>, vector<int>> BFS_from_cluster_on_complement_graph(vector<int> cluster) {assert(is_prepared); vector<int> dst(V, -1), pr(V, -1); for (int i : cluster) dst[i] = 0; vector<int> s; for (int q = 0; q < V; ++q) if (dst[q] != 0) s.push_back(q); deque<int> dq(cluster.begin(), cluster.end()); vector<bool> tyt(V); while (dq.size()) {int v = dq.front(); dq.pop_front(); for (const auto& e : (*this)[v]) tyt[e.to] = 1; for (int q = 0; q < s.size(); ++q) {const int h = s[q]; if (!tyt[h]) {dst[h] = dst[v] + 1; pr[h] = v; dq.push_back(h); if (q + 1 < s.size()) swap(s[q], s.back()); s.pop_back(); --q;}} for (const auto& e : (*this)[v]) tyt[e.to] = 0;} return {dst, pr};}

    vector<int> maximal_independent_set_greedy() {assert(is_prepared); vector<int> dg(V); set<pair<int, int>> s; for (int q = 0; q < V; ++q) {dg[q] = deg(q); s.insert({dg[q], q});} vector<int> ans; while (s.size()) {auto [d, v] = *s.begin(); s.erase(s.begin()); ans.push_back(v); dg[v] = -1; for (const auto& e : (*this)[v]) {if (dg[e.to] != -1) {s.erase({dg[e.to], e.to}); dg[e.to] = -2;}} for (const auto& e : (*this)[v]) {if (dg[e.to] == -2) {for (const auto& ee : (*this)[e.to]) {if (dg[ee.to] > 0) {s.erase({dg[ee.to], ee.to}); --dg[ee.to]; s.insert({dg[ee.to], ee.to});}} dg[e.to] = -1;}}} return ans;}
};
//Usage: graph<edge<T>> g(V, [E]), where T is the type of info you want to store on edges, T could be void.
//Before using graph, add all edges and then call g.prepare()
