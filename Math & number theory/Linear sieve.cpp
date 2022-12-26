//Returns vector of minimal divisors for numbers [0; n]
//O(n)
vector<int> min_divisors(int n) {
    vector<int> min_div(n + 1), primes;
    for (int q = 2; q <= n; ++q) {
        if (min_div[q] == 0) {
            primes.push_back(q);
            min_div[q] = q;
        }
        for (int p : primes) {
            int c = p * q;
            if (p > min_div[q] || c >= n) break;
            min_div[c] = p;
        }
    }
    return min_div;
}

//Example: n = 60 -> {{2, 2}, {3, 1}, {5, 1}}
//O(# of multipliers in the factorization of n) <= O(log_2(n))
vector<pair<int, int>> get_factorization(vector<int>& min_div, int n) {
    assert(1 <= n && n < min_div.size());
    vector<pair<int, int>> ans;
    for (; n != 1;) {
        int mnd = min_div[n], cnt = 0;
        for (; min_div[n] == mnd; n /= mnd) {
            ++cnt;
        }
        ans.emplace_back(mnd, cnt);
    }
    return ans;
}