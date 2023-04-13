//O(n)
vector<pair<int, int>> prufer_code_to_tree(vector<int> pcode) {
    const int n = pcode.size() + 2;
    vector<int> cnt(n);
    for (int v : pcode) ++cnt[v];
    vector<pair<int, int>> ans(n - 1);
    int leaf = find(cnt.begin(), cnt.end(), 0) - cnt.begin();
    for (int i = 0, ptr = leaf + 1; i < n - 2; ++i) {
        int p = pcode[i];
        ans[i] = {p, leaf};
        if (--cnt[p] == 0 && p < ptr) leaf = p;
        else {
            while (cnt[ptr]) ++ptr;
            leaf = ptr++;
        }
    }
    ans.back() = {leaf, n - 1};
    return ans;
}