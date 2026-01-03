//Floyd-Warshall algorithm
//Returns matrix d s. t. d[i][j] = shortest distance from i to j
//Important: most likely you want g[i][i] to be 0
//Complexity: O(n ^ 3)
template<typename T>
vector<vector<T>> floyd_warshall(const vector<vector<T>>& g) {
    const size_t n = g.size();
    for (auto& i : g) assert(i.size() == n);
    auto d = g;
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
            }
        }
    }
    return d;
}
