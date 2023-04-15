//Returns {dst, prv}
//dst - distances from cluster
//prv - previous nodes on shortest path from cluster
//dst[v] = inf if no path from cluster to v
//prv[v] = -1  if no path from cluster to v or v is in cluster
template<typename T>
pair<vector<T>, vector<int>> dijkstra(graph<edge<T>>& g, const vector<int>& cluster, const T inf = numeric_limits<T>::max()) {
    const int n = g.size();
    vector<T> dst(n, inf);
    vector<int> pr(n, -1);
    auto cmp = [](const pair<int, T>& p1, const pair<int, T>& p2) {return p1.second > p2.second;};
    priority_queue<pair<int, T>, vector<pair<int, T>>, decltype(cmp)> pq(cmp);
    for (int v : cluster) dst[v] = 0, pq.emplace(v, 0);
    while (!pq.empty()) {
        auto [v, cur] = pq.top(); pq.pop();
        if (cur != dst[v]) continue;
        for (const auto& [h, d] : g[v]) {
            if (cur + d < dst[h]) {
                dst[h] = cur + d;
                pr[h] = v;
                pq.emplace(h, dst[h]);
            }
        }
    }
    return {dst, pr};
}