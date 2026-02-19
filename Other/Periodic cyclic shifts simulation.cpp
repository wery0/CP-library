/*
For a given array of length n and number k (k divides n) performs following operations and returns resulting array:
  1. Simultaneous cyclic shift of each k-length block on x
  2. Cyclic shift of whole array on x
Complexity: O(n + q)
Constraints: sum of all x'es should fit into C
*/
template<typename T, typename C>
vector<T> periodic_cyclic_shifts_simulation(vector<T> a, int k, vector<pair<int, C>> queries) {
    const int n = a.size(), b = n / k;
    assert(n % k == 0);
    int st = 0;
    vector<C> sh(k + 1);
    for (auto [t, x] : queries) {
        if (t == 1) {
            st = (st + x % k + k) % k;
        } else if (t == 2) {
            x = (x % n + n) % n;
            x = (n - x) % n;
            st = (st + k - x % k) % k;
            sh[0] += x / k;
            if (int eso = x % k; eso) {
                ++sh[st];
                --sh[st + min(eso, k - st)];
                eso -= min(eso, k - st);
                ++sh[0], --sh[eso];
            }
        } else assert(0);
    }
    partial_sum(sh.begin(), sh.end(), sh.begin());
    for (auto& c : sh) c %= b;
    vector<T> res(n);
    for (int i = 0; i < k; ++i) {
        int x = (st + i) % k;
        for (int j = 0; j < b; ++j) {
            int y = (j - sh[x] + b) % b;
            res[j * k + i] = a[x + y * k];
        }
    }
    return res;
}
