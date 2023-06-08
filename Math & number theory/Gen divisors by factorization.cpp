//Example: f({2, 2, 3, 5}) = {1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60}
//O(#divisors) for their generation, O((#divisors)log(#divisors)) for their sorting afterwards.
template<typename T>
vector<T> get_divisors_by_factorization(vector<T> factorization) {
    sort(factorization.begin(), factorization.end());
    vector<T> dvz, cnt;
    for (size_t i = 0; i < factorization.size(); ++i) {
        if (i == 0 || factorization[i] != dvz.back()) {
            dvz.push_back(factorization[i]);
            cnt.push_back(1);
        } else ++cnt.back();
    }
    T nd = 1;
    for (T c : cnt) nd *= c + 1;
    vector<T> ans(nd), kek(dvz.size());
    for (T ii = 0, c = 1; ii < nd; ++ii) {
        ans[ii] = c;
        for (size_t i = 0; i < dvz.size(); ++i) {
            if (++kek[i] <= cnt[i]) {
                c *= dvz[i];
                break;
            }
            while (--kek[i]) c /= dvz[i];
        }
    }
    sort(ans.begin(), ans.end());         //Remove this line if the order of divisors doesn't matter.
    return ans;
}