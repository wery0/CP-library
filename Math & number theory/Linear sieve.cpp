/*
Returns array of vectors {min_div, cnt_div, sum_div, eul_fun}, where
    min_div[i] = minimum divisor of i
    cnt_div[i] = # divisors of i
    sum_div[i] = sum of divisors of i
    eul_fun[i] = phi(n)
and i \in [0; n]
*/
//O(n)
template<typename T>
array<vector<T>, 4> linear_sieve(int n) {
    vector<T> primes;
    vector<T> min_div(n + 1);
    vector<T> cnt_div(n + 1, 1);
    vector<T> sum_div(n + 1, 1);           //Be careful with overflow
    vector<T> eul_fun(n + 1, 1);
    vector<T> pow_of_min_div(n + 1);
    for (int i = 2; i <= n; ++i) {
        if (min_div[i] == 0) {
            primes.push_back(i);
            min_div[i] = i;
            cnt_div[i] = 2;
            sum_div[i] = i + 1;
            eul_fun[i] = i - 1;
            pow_of_min_div[i] = 1;
        }
        for (T p : primes) {
            T c = p * i;
            if (p > min_div[i] || c > n) break;
            min_div[c] = p;
            pow_of_min_div[c] = 1 + (p == min_div[i] ? pow_of_min_div[i] : 0);
            T pw = 1; for (int j = 0; j < pow_of_min_div[c]; ++j) pw *= p;
            T base = c / pw;
            cnt_div[c] = cnt_div[base] * (1 + pow_of_min_div[c]);
            sum_div[c] = sum_div[base] * (pw * p - 1) / (p - 1);       //Be careful with overflow
            eul_fun[c] = eul_fun[base] * (pw - pw / p);
        }
    }
    return {min_div, cnt_div, sum_div, eul_fun};
}

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
