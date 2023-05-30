//Returns vector of minimal divisors for numbers [0; n]
//O(n)
vector<int> min_divisors(int n) {
    vector<int> min_div(n + 1), primes;
    for (int i = 2; i <= n; ++i) {
        if (min_div[i] == 0) {
            primes.push_back(i);
            min_div[i] = i;
        }
        for (int p : primes) {
            int c = p * i;
            if (p > min_div[i] || c > n) break;
            min_div[c] = p;
        }
    }
    return min_div;
}

//Example: n = 60 -> {{2, 2}, {3, 1}, {5, 1}}
//O(# of multipliers in the factorization of n) <= O(log_2(n))
vector<array<int, 2>> get_factorization(vector<int>& min_div, int n) {
    assert(1 <= n && n < min_div.size());
    vector<array<int, 2>> ans;
    while (n != 1) {
        int mnd = min_div[n], cnt = 0;
        for (; min_div[n] == mnd; n /= mnd) {
            ++cnt;
        }
        ans.push_back({mnd, cnt});
    }
    return ans;
}