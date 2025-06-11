//Divides an array into the minimum number of good subsegments
//Returns borders of subsegments, if partition exist
//Complexity: at most O(n ^ 2) is_good() calls 
template <typename T_arr>
vector<array<int, 2>> min_subsegments(const T_arr& arr) {
    const size_t n = arr.size();
    auto is_good = [&](int l, int r) -> bool {
        //write
    };
    vector<int> dp(n + 1);
    vector<int> pr(n + 1, -1);
    vector<int> good_l = {1};
    for (int r = 1; r <= n; ++r) {
        dp[r] = r + 1;
        for (int l : good_l) {
            if (!is_good(l - 1, r - 1)) {
                continue;
            }
            int tyt = dp[l - 1] + 1;
            if (tyt < dp[r]) {
                dp[r] = tyt;
                pr[r] = l - 1;
            }
        }
        if (pr[r] != -1) {
            good_l.push_back(r + 1);
        }
    }
    if (pr[n] == -1) return {};
    vector<array<int, 2>> ans(dp[n]);
    for (int r = n, i = dp[n]; r; r = pr[r]) {
        ans[--i] = {pr[r], r - 1};
    }
    return ans;
}
