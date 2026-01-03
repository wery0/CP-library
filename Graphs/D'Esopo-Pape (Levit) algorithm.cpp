//D'Esopo-Pape (aka Levit's) algorithm
//Returns {dst, prv}
//dst - distances from cluster
//prv - previous nodes on shortest path from cluster
//dst[v] = inf if no path from cluster to v
//prv[v] = -1  if no path from cluster to v or v is in cluster
template<typename T>
pair<vector<T>, vector<int>> levit(vector<vector<pair<int, T>>>& g, const vector<int>& cluster, const T inf = numeric_limits<T>::max()) {
    const int n = g.size();
    vector<T> dst(n, inf);
    vector<int> pr(n, -1), m(n, 2);
    for (int v : cluster) dst[v] = 0, m[v] = 1;
    deque<int> dq(cluster.begin(), cluster.end());
    for (auto& gv : g) {
        sort(gv.begin(), gv.end(), [](const auto& l, const auto& r){return l.second < r.second;});
    }
    while (dq.size()) {
        int v = dq.front(); dq.pop_front(); m[v] = 0;
        for (const auto& [h, d] : g[v]) {
            if (dst[v] + d < dst[h]) {
                dst[h] = dst[v] + d;
                pr[h] = v;
                if (m[h] == 2) {
                    dq.push_back(h); m[h] = 1;
                } else if (m[h] == 0) {
                    dq.push_front(h); m[h] = 1;
                }
            }
        }   
    }
    return {dst, pr};
}
