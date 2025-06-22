//Returns # good numbers in range [l, r], where l > 0
template<typename T>
T count_good_numbers_in_range_two_statistics(string l, string r) {
    //Returns # good numbers in range [1, num]
    auto calc_pref = [&](string num, bool lst) -> T {
        const int MAX_STATISTIC1 = ?;
        const int MAX_STATISTIC2 = ?;
        auto f1 = [](int statistic, int digit) {return ?;};
        auto f2 = [](int statistic, int digit) {return ?;};
        const size_t n = num.size();
        for (auto& c : num) c -= '0';
        //dp[i][s1][s2][k] = # good numbers of length exactly i, with first statistic s1, second statistic s2, and relation to num is k.
        vector dp(2, vector(MAX_STATISTIC1, vector(MAX_STATISTIC2, array<T, 3>{0, 0, 0})));
        for (int digit = 1, gr = num.size() == 1 ? num[0] : 9; digit <= gr; ++digit) {
            const int _ = digit < num[0] ? 0 : digit == num[0] ? 1 : 2;
            const int s1 = f1(?, digit);
            const int s2 = f2(?, digit);
            if (s1 < 0 || s1 >= MAX_STATISTIC1) continue;
            if (s2 < 0 || s2 >= MAX_STATISTIC2) continue;
            dp[0][s1][s2][_] += 1;
        }
        T o = 0;
        for (int i = 0; i < n; ++i) {
            const int I = i & 1;
            const int G = i + 1 < n ? 3 : lst ? 2 : 1;
            for (int s1 = 0; s1 < ?; ++s1) {
                for (int s2 = 0; s2 < ?; ++s2) {
                    for (int _ = 0; _ < G; ++_) {
                        o += dp[I][s1][s2][_];
                    }
                }
            }
            if (i + 1 == n) break;
            for (int s1 = 0; s1 < MAX_STATISTIC1; ++s1) {
                for (int s2 = 0; s2 < MAX_STATISTIC2; ++s2) {
                    for (int u = 0; u < 3; ++u) {
                        dp[I ^ 1][s1][s2][u] = 0;
                    }
                }
            }
            for (int s1 = 0; s1 < MAX_STATISTIC1; ++s1) {
                for (int s2 = 0; s2 < MAX_STATISTIC2; ++s2) {
                    for (int u = 0; u < 3; ++u) {
                        if (dp[I][s1][s2][u] == 0) continue;
                        for (int digit = 0; digit < 10; ++digit) {
                            const int _ = digit < num[i + 1] ? 0 : digit == num[i + 1] ? 1 : 2;
                            const int ns1 = f1(s1, digit);
                            const int ns2 = f2(s2, digit);
                            if (digit < 0 || digit > 9) continue;
                            if (ns1 < 0 || ns1 >= MAX_STATISTIC1) continue;
                            if (ns2 < 0 || ns2 >= MAX_STATISTIC2) continue;
                            dp[I ^ 1][ns1][ns2][u == 1 ? _ : u] += dp[I][s1][s2][u];
                        }
                    }
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
