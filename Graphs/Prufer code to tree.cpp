//O(n)
vector<pair<int, int>> prufer_code_to_tree(vector<int> pcode) {
    int n = pcode.size() + 2;
    vector<int> cnt(n);
    for (int v : pcode) ++cnt[v];
    vector<pair<int, int>> ans(n - 1);
    int leaf = leaf = find(all(cnt), 0) - cnt.begin();
    for (int q = 0, ptr = leaf + 1; q < n - 2; q++) {
        int p = pcode[q];
        ans[q] = {p, leaf};
        if (--cnt[p] == 0 && p < ptr) {
            leaf = p;
        } else {
            for (; cnt[ptr]; ) ++ptr;
            leaf = ptr++;
        }
    }
    ans.back() = {leaf, n - 1};
    return ans;
}