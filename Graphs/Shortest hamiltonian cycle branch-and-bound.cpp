//Finds shortest hamiltonian cycle using branch-and-bound optimization
//Returns {length of shortest cycle, cycle itself}
template<typename T>
pair<T, vector<int>> shortest_hamiltonian_cycle_branch_and_bound(const vector<vector<T>>& m) {
    static constexpr T INF = numeric_limits<T>::max();
    const size_t n = m.size();
    if (n == 0) return {0, {}};
    vector<int> best_cyc;
    T best_len = INF;
    auto estimate_min_len = [&](const vector<int>& rv) -> T {
        size_t N = rv.size();
        if (N <= 1) return 0;
        vector<int> us(N), bv(N);
        T res = 0;
        us[0] = 1;
        for (int i = 1; i < N; ++i) {
            int h = -1;
            T bl = INF;
            for (int v = 0; v < N; ++v) {
                if (us[v]) continue;
                if (m[rv[v]][rv[bv[v]]] < bl) {
                    bl = m[rv[v]][rv[bv[v]]];
                    h = v;
                }
            }
            us[h] = 1;
            res += bl;
            for (int v = 0; v < N; ++v) {
                if (us[v]) continue;
                if (m[rv[v]][rv[h]] < m[rv[v]][rv[bv[v]]]) bv[v] = h;
            }
        }
        return res;
    };
    vector<int> cur_path = {0}, rv(n - 1);
    iota(rv.begin(), rv.end(), 1);
    function<void(T)> f = [&](T cur_l) {
        assert(cur_path.size());
        int last_v = cur_path.back();
        if (rv.empty()) cur_l += m[last_v][cur_path[0]];
        if (cur_l + estimate_min_len(rv) * (rv.size() > 3 ? 1 : 1) >= best_len) return;
        if (rv.empty()) {
            best_len = cur_l;
            best_cyc = cur_path;
        }
        vector<int> vrv(rv.begin(), rv.end());
        sort(vrv.begin(), vrv.end(), [&](int l, int r) {return m[last_v][l] < m[last_v][r];});
        for (int nv : vrv) {
            cur_path.push_back(nv);
            rv.erase(find(rv.begin(), rv.end(), nv));
            f(cur_l + m[last_v][nv]);
            rv.push_back(nv);
            cur_path.pop_back();
        }
    };
    f(0);
    rotate(best_cyc.begin(), min_element(best_cyc.begin(), best_cyc.end()), best_cyc.end());
    assert(best_cyc.size() == n);
    return {best_len, best_cyc};
}
