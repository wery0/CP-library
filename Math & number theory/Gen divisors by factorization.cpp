template<typename T>
vector<T> get_divisors_by_factorization(vector<T> fc) {
    sort(fc.begin(), fc.end());
    vector<T> dvz, cnt;
    for (int i = 0; i < fc.size(); ++i) {
        if (i == 0 || fc[i] != dvz.back()) {
            dvz.push_back(fc[i]);
            cnt.push_back(1);
        } else ++cnt.back();
    }
    T nd = 1;
    for (T c : cnt) nd *= c + 1;
    vector<T> ans(nd), kek(dvz.size());
    for (ll ii = 0, c = 1; ii < nd; ++ii) {
        ans[ii] = c;
        for (ll i = 0; i < dvz.size(); ++i) {
            if (++kek[i] <= cnt[i]) {
                c *= dvz[i];
                break;
            }
            while (--kek[i]) c /= dvz[i];
        }
    }
    sort(ans.begin(), ans.end());
    return ans;
}