//Returns # good numbers in range [l, r], where l > 0
template<typename T>
T count_good_numbers_in_range_one_statistic(string l, string r) {
    //Returns # good numbers in range [1, num]
    auto calc_pref = [&](string num, bool lst) -> T {
        const int MAX_STATISTIC = ?;
        auto f = [](int statistic, int digit) {return ?;};
        const size_t n = num.size();
        const int gr = num.size() == 1 ? stoi(num) : 9;
        for (auto& c : num) c -= '0';
        //dp[i][s][k] = # good numbers of length exactly i, with statistic s, and relation to num is k.
        vector dp(n, vector(MAX_STATISTIC, array<T, 3>{0, 0, 0}));
        for (int digit = 1; digit <= gr; ++digit) {
            const int _ = digit < num[0] ? 0 : digit == num[0] ? 1 : 2;
            const int s = f(?, digit);
            if (s < 0 || s >= MAX_STATISTIC) continue;
            dp[0][s][_] += 1;
        }
        for (int i = 0; i + 1 < n; ++i) {
            for (int s = 0; s < MAX_STATISTIC; ++s) {
                for (int u = 0; u < 3; ++u) {
                    if (dp[i][s][u] == 0) continue;
                    for (int digit = 0; digit < 10; ++digit) {
                        const int _ = digit < num[i + 1] ? 0 : digit == num[i + 1] ? 1 : 2;
                        const int ns = f(s, digit);
                        if (digit < 0 || digit > 9) continue;
                        if (ns < 0 || ns >= MAX_STATISTIC) continue;
                        dp[i + 1][ns][u == 1 ? _ : u] += dp[i][s][u];
                    }
                }
            }
        }
        T o = 0;
        for (int i = 0; i < n; ++i) {
            const int G = i + 1 < n ? 3 : lst ? 2 : 1;
            for (int s = 0; s < MAX_STATISTIC; ++s) {
                for (int j = 0; j < G; ++j) {
                    o += dp[i][s][j];
                }
            }
        }
        return o;
    };
    auto simplify = [](string& s) {
        assert(!s.empty());
        for (auto& c : s) assert(isdigit(c));
        auto pos = s.find_first_not_of('0');
        if (pos == string::npos) s = "0";
        else s.erase(s.begin(), s.begin() + pos);
    };
    simplify(l);
    simplify(r);
    if (l.size() > r.size() || l.size() == r.size() && l > r) return 0;
    assert(l != "0");
    return calc_pref(r, 1) - calc_pref(l, 0);
}
