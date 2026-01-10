//Shortest path faster algorithm (aka Ford-Bellman with queue)
//Returns {dst, prv}
//dst - distances from cluster
//prv - previous nodes on shortest path from cluster
//dst[v] = inf if no path from cluster to v
//prv[v] = -1  if no path from cluster to v or v is in cluster
template<typename T>
pair<vector<T>, vector<int>> SPFA(vector<vector<pair<int, T>>>& g, const vector<int>& cluster, const T inf = numeric_limits<T>::max()) {
    const int n = g.size();
    vector<T> dst(n, inf);
    vector<int> pr(n, -1);
    vector<bool> inq(n);
    deque<int> dq(cluster.begin(), cluster.end());
    for (int v : cluster) dst[v] = 0, inq[v] = 1;
    for (auto& gv : g) {
        sort(gv.begin(), gv.end(), [](const auto& l, const auto& r){return l.second < r.second;});
    }
    while (!dq.empty()) {
        // constexpr size_t MAGIC = 2;
        // for (size_t i = 0; i < MAGIC && i + 1 < dq.size(); ++i) {
        //     if (dst[dq.back()] < dst[dq[i]]) {
        //         swap(dq.back(), dq[i]);
        //         break;
        //     }
        // }
        int v = dq.front(); dq.pop_front();
        inq[v] = 0;
        const T cur = dst[v];
        for (const auto& [h, d] : g[v]) {
            if (cur + d < dst[h]) {
                dst[h] = cur + d;
                pr[h] = v;
                if (!inq[h]) {
                    inq[h] = 1;
                    dq.push_back(h);
                }
            }
        }
    }
    return {dst, pr};
}
