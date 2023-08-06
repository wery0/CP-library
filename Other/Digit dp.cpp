//Forward dp. dp[i][j][k] = # good numbers of length exactly i, with last digit j, and relation to s is k. 
mint f(string s, bool lst) {
    const int M2 = max(10, (int)?);
    assert(!s.empty());
    const size_t a = s.size();
    for (auto& c : s) c -= '0';
    vector<vector<vector<mint>>> dp(a, vector<vector<mint>>(M2, vector<mint>(3)));
    for (int j = 1; j < 10; ++j) {
        dp[0][j][j < s[0] ? 0 : j == s[0] ? 1 : 2] = 1;
    }
    for (int i = 0; i + 1 < a; ++i) {
        for (int j = 0; j < M2; ++j) {
            for (int nd = 0; nd < 10; ++nd) {
                if (nd < 0 || nd > 9) continue;
                const int nj = ?;
                dp[i + 1][nj][0] += dp[i][j][0];
                dp[i + 1][nj][nd < s[i + 1] ? 0 : nd == s[i + 1] ? 1 : 2] += dp[i][j][1];
                dp[i + 1][nj][2] += dp[i][j][2];
            }
        }
    }
    mint o = 0;
    for (int i = 0; i + 1 < a; ++i) {
        for (int j = 0; j < M2; ++j) {
            o += dp[i][j][0];
            o += dp[i][j][1];
            o += dp[i][j][2];
        }
    }
    for (int j = 0; j < M2; ++j) {
        o += dp[a - 1][j][0];
        if (lst) o += dp[a - 1][j][1];
    }
    return o;
}

//Counts # good numbers in range [l, r]
mint count_lr(string l, string r) {
    return f(r, 1) - f(l, 0);
}