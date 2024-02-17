template<typename T>
class functional_graph {

    size_t V;

    pair<size_t, T> jump_inside_tree(size_t v, int64_t k) {
        assert(k <= depth_in_tree[v]);
        T sm = 0;
        while (k) {
            int jump_len = depth_in_tree[v] - depth_in_tree[jump[v]];
            if (jump_len <= k) {
                sm += jump_sum[v];
                v = jump[v], k -= jump_len;
            } else {
                sm += val[v];
                v = next[v], --k;
            }
        }
        sm += val[v];
        return {v, sm};
    }

    pair<size_t, T> move_in_cyc(size_t v, int64_t k) {
        assert(oncyc[v]);
        auto [full_cycles, rem_in_cyc] = lldiv(k, len_of_cyc[v]);
        int res_vertex = cycs[num_cyc[v]][(pos_in_cyc[v] + rem_in_cyc) % len_of_cyc[v]];
        T res_sum = sm_of_cyc[v] * full_cycles + seg_sum(num_cyc[v], pos_in_cyc[v], pos_in_cyc[res_vertex]);
        return {res_vertex, res_sum};
    }

    T seg_sum(size_t i, size_t l, size_t r) {
        if (l <= r) return pref_sums_cycs[i][r] - (l ? pref_sums_cycs[i][l - 1] : 0);
        return seg_sum(i, l, pref_sums_cycs[i].size() - 1) + seg_sum(i, 0, r);
    }

    void init() {
        dst_to_end.resize(V, -1);
        sm_to_end.resize(V);
        oncyc.resize(V);
        jump.resize(V);
        jump_sum.resize(V);
        first_on_cyc.resize(V, -1);
        dst_to_cyc.resize(V, -1);
        sm_to_cyc.resize(V, -1);
        sm_of_cyc.resize(V, -1);
        len_of_cyc.resize(V, -1);
        num_cyc.resize(V, -1);
        pos_in_cyc.resize(V, -1);
        root_of_tree.resize(V, -1);
        depth_in_tree.resize(V, -1);
        inv.resize(V);
        for (size_t v = 0; v < V; ++v) {
            if (next[v] != -1) {
                inv[next[v]].push_back(v);
            }
        }
        vector<int> us(V);
        for (int st = 0; st < V; ++st) {
            if (us[st]) continue;
            stack<int> stack;
            for (int v = st; ; v = next[v]) {
                if (v == -1) {
                    for (T sm = 0, d = 0; stack.size(); ++d) {
                        int x = stack.top(); stack.pop();
                        us[x] = 2;
                        sm += val[x];
                        dst_to_end[x] = d;
                        sm_to_end[x] = sm;
                    }
                    break;
                }
                if (us[v] == 1) {
                    T smc = 0;
                    vector<int> cyc;
                    while (true) {
                        int x = stack.top(); stack.pop();
                        cyc.push_back(x);
                        smc += val[x];
                        oncyc[x] = 1;
                        us[x] = 2;
                        if (x == v) break;
                    }
                    reverse(cyc.begin(), cyc.end());
                    for (size_t i = 0; i < cyc.size(); ++i) {
                        pos_in_cyc[cyc[i]] = i;
                    }
                    for (auto x : cyc) {
                        sm_of_cyc[x] = smc;
                        len_of_cyc[x] = cyc.size();
                        num_cyc[x] = cycs.size();
                    }
                    cycs.push_back(cyc);
                    for (T sm = 0, d = 1; stack.size(); ++d) {
                        int x = stack.top(); stack.pop();
                        us[x] = 2;
                        sm += val[x];
                        dst_to_cyc[x] = d;
                        first_on_cyc[x] = v;
                        sm_to_cyc[x] = sm;
                    }
                    break;
                }
                if (us[v] == 2) {
                    if (oncyc[v]) {
                        for (T sm = 0, d = 1; stack.size(); ++d) {
                            int x = stack.top(); stack.pop();
                            us[x] = 2;
                            sm += val[x];
                            dst_to_cyc[x] = d;
                            first_on_cyc[x] = v;
                            sm_to_cyc[x] = sm;
                        }
                    } else if (first_on_cyc[v] != -1) {
                        for (T sm = sm_to_cyc[v], d = dst_to_cyc[v] + 1; stack.size(); ++d) {
                            int x = stack.top(); stack.pop();
                            us[x] = 2;
                            sm += val[x];
                            dst_to_cyc[x] = d;
                            first_on_cyc[x] = first_on_cyc[v];
                            sm_to_cyc[x] = sm;
                        }
                    } else {
                        for (T sm = sm_to_end[v], d = dst_to_end[v] + 1; stack.size(); ++d) {
                            int x = stack.top(); stack.pop();
                            us[x] = 2;
                            sm += val[x];
                            dst_to_end[x] = d;
                            sm_to_end[x] = sm;
                        }
                    }
                    break;
                }
                us[v] = 1;
                stack.push(v);
            }
        }
        auto dfs = [&](auto&& dfs, int v, int r = -1) -> void {
            if (r == -1) r = v;
            root_of_tree[v] = r;
            depth_in_tree[v] = v == r ? 0 : depth_in_tree[next[v]] + 1;
            jump_sum[v] = val[v];
            if (depth_in_tree[v] == 0) {
                jump[v] = v;
            } else if (depth_in_tree[next[v]] + depth_in_tree[jump[jump[next[v]]]] == depth_in_tree[jump[next[v]]] * 2) {
                jump[v] = jump[jump[next[v]]];
                if (next[v] != root_of_tree[v]) jump_sum[v] += jump_sum[next[v]];
                if (jump[next[v]] != next[v]) jump_sum[v] += jump_sum[jump[next[v]]];

            } else {
                jump[v] = next[v];
            }
            for (int h : inv[v]) {
                dfs(dfs, h, r);
            }
        };
        for (size_t v = 0; v < V; ++v) {
            if (dst_to_end[v] == 0) {
                dfs(dfs, v);
            } else {
                if (!oncyc[v]) continue;
                for (int h : inv[v]) {
                    if (oncyc[h]) continue;
                    dfs(dfs, h);
                }
            }
        }
        pref_sums_cycs.resize(cycs.size());
        for (size_t i = 0; i < cycs.size(); ++i) {
            pref_sums_cycs[i].resize(cycs[i].size());
            for (size_t j = 0; j < cycs[i].size(); ++j) {
                pref_sums_cycs[i][j] = val[cycs[i][j]];
            }
            partial_sum(pref_sums_cycs[i].begin(), pref_sums_cycs[i].end(), pref_sums_cycs[i].begin());
        }
    }

public:
    vector<int> next;
    vector<T> val;
    vector<int> dst_to_end;
    vector<int> oncyc;
    vector<int> first_on_cyc;
    vector<int> dst_to_cyc;
    vector<T> sm_to_end;
    vector<T> sm_to_cyc;
    vector<T> sm_of_cyc;
    vector<T> jump_sum;
    vector<int> len_of_cyc;
    vector<int> num_cyc;
    vector<int> pos_in_cyc;
    vector<int> jump;
    vector<int> root_of_tree;
    vector<int> depth_in_tree;
    vector<vector<int>> inv;
    vector<vector<int>> cycs;
    vector<vector<T>> pref_sums_cycs;

    functional_graph() = default;

    functional_graph(vector<int> next, vector<T> val): V(next.size()), next(next), val(val) {
        assert(next.size() == val.size());
        init();
    }

    //Returns next[next[...next[v]]] (k times) and sum of vals of all vertexes in the path
    //O(log(n))
    pair<int, T> make_k_steps(size_t v, int64_t k) {
        assert(k >= 0);
        T sm = 0;
        if (!oncyc[v]) {
            if (first_on_cyc[v] == -1) {
                if (k >= dst_to_end[v]) {
                    return {k == dst_to_end[v] ? root_of_tree[v] : -1, sm_to_end[v]};
                } else {
                    return jump_inside_tree(v, k);
                }
            } else {
                if (k >= dst_to_cyc[v]) {
                    k -= dst_to_cyc[v];
                    sm += sm_to_cyc[v];
                    v = first_on_cyc[v];
                } else {
                    return jump_inside_tree(v, k);
                }
            }
        }
        auto [rv, rs] = move_in_cyc(v, k);
        return {rv, sm + rs};
    }
};
