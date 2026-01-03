//Tarjan algorithm (improved SPFA) https://codeforces.com/blog/entry/3793?#comment-77109
//Returns {dst, prv}
//dst - distances from cluster
//prv - previous nodes on shortest path from cluster
//dst[v] = inf if no path from cluster to v
//prv[v] = -1  if no path from cluster to v or v is in cluster
template<typename T>
pair<vector<T>, vector<int>> SPFA_tarjan(vector<vector<pair<int, T>>>& g, const vector<int>& cluster, const T inf = numeric_limits<T>::max()) {
    const int n = g.size();
    vector<T> dst(n, inf);
    vector<int> pr(n, -1), nxt(n), prv(n), deg(n), type(n), p(n, -1);
    deque<int> dq(cluster.begin(), cluster.end());
    for (int v : cluster) {
        dst[v] = 0;
        type[v] = 1;
        nxt[v] = prv[v] = v;
    }
    for (auto& gv : g) {
        sort(gv.begin(), gv.end(), [](const auto& l, const auto& r){return l.second < r.second;});
    }
    while (dq.size()) {
        // constexpr size_t MAGIC = 2;
        // for (size_t i = 0; i < MAGIC && i + 1 < dq.size(); ++i) {
        //     if (dst[dq.back()] < dst[dq[i]]) {
        //         swap(dq.back(), dq[i]);
        //         break;
        //     }
        // }
        int v = dq.front(); dq.pop_front();
        if (type[v] == 0) continue;
        for (auto [h, dd] : g[v]) {
            if (dst[v] + dd >= dst[h]) continue;
            T delta = dst[h] - dst[v] - dd;
            dst[h] = dst[v] + dd;
            pr[h] = v;
            if (type[h] != 1) dq.push_back(h);
            if (type[h] != 0) {
                int cur = nxt[h], score = deg[h];
                while (score != 0) {
                    type[cur] = 0;
                    dst[cur] -= delta - 1;
                    score += deg[cur] - 1;
                    cur = nxt[cur];
                }
                nxt[prv[h]] = cur;
                prv[cur] = prv[h];
                if (p[h] != -1) deg[p[h]]--;
            }
            prv[h] = v; nxt[h] = nxt[v];
            prv[nxt[h]] = h; nxt[prv[h]] = h;
            deg[h] = 0; deg[v]++;
            type[h] = 1;
            p[h] = v;
        }
        type[v] = 2;
    }
    return {dst, pr};
}
