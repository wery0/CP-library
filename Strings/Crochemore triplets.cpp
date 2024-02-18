//Returns all runs (aka crochemore triplets) of arr as vector of {p, l, r}, which means that period of s[l, r] is p
//O(n) calls of lcp and lcs + O(sorting(runs))
template<typename T_arr>
vector<array<int, 3>> calculate_runs(const T_arr& arr) {
    const size_t n = arr.size();
    vector<int> st(n + 1);
    vector<array<int, 3>> runs;
    hasher64 kek(arr.begin(), arr.end());
    // hasher_pow2<64> kek(arr.begin(), arr.end());  //Be careful with thue-morse strings
    for (int inv = 0; inv < 2; ++inv) {
        st[0] = n + 1;
        for (int i = n, top = 0, lt = 0; i; --i) {
            while (top) {
                int x = min(st[top] - i, st[top - 1] - st[top]);
                lt = kek.lcp(i - 1, st[top] - 1, x);
                if ((lt == x && st[top] - i < st[top - 1] - st[top]) ||
                        (lt < x && ((arr[i + lt - 1] < arr[st[top] + lt - 1]) ^ inv))) {
                    --top, lt = 0;
                } else {
                    break;
                }
            }
            int j = st[top], x = min(i, j) > 1 ? kek.lcs(i - 2, j - 2, j - i) : 0;
            st[++top] = i;
            if (x < j - i) {
                int y = lt + (max(i, j) + lt - 1 < n ? kek.lcp(i + lt - 1, j + lt - 1) : 0);
                if (x + y >= j - i) {
                    runs.push_back({j - i, i - x - 1, j + y - 2});
                }
            }
        }
    }
    sort(runs.begin(), runs.end());
    runs.erase(unique(runs.begin(), runs.end()), runs.end());
    return runs;
}
