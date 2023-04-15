//Shortest path faster algorithm (aka Ford-Bellman with queue)
//Returns {dst, prv}
//dst - distances from cluster
//prv - previous nodes on shortest path from cluster
//dst[v] = inf if no path from cluster to v
//prv[v] = -1  if no path from cluster to v or v is in cluster
template<typename T>
pair<vector<T>, vector<int>> SPFA(const vector<vector<pair<int, T>>>& g, const vector<int>& cluster, const T inf = numeric_limits<T>::max()) {
    const int n = g.size();
    vector<T> dst(n, inf);
    vector<int> pr(n, -1);
    vector<bool> inq(n);
    deque<int> que(cluster.begin(), cluster.end());
    for (int v : cluster) dst[v] = 0, inq[v] = 1;
    while (!que.empty()) {
        // constexpr size_t MAGIC = 2;
        // for (size_t i = 0; i < MAGIC && i + 1 < que.size(); ++i) {
        //     if (dst[que.back()] < dst[que[i]]) {
        //         swap(que.back(), que[i]);
        //         break;
        //     }
        // }
        int v = que.front(); que.pop_front();
        inq[v] = 0;
        const T cur = dst[v];
        for (const auto& [h, d] : g[v]) {
            if (cur + d < dst[h]) {
                dst[h] = cur + d;
                pr[h] = v;
                if (!inq[h]) {
                    inq[h] = 1;
                    que.push_back(h);
                }
            }
        }
    }
    return {dst, pr};
}