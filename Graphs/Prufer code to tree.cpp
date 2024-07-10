//Returns vector of edges of corresponding tree
//O(n)
vector<pair<int, int>> prufer_code_to_tree(const vector<int>& pcode) {
    const size_t n = pcode.size() + 2;
    vector<int> cnt(n);
    for (int v : pcode) {
        assert(0 <= v && v < n);
        ++cnt[v];
    }
    vector<pair<int, int>> ans(n - 1);
    int leaf = find(cnt.begin(), cnt.end(), 0) - cnt.begin();
    for (size_t i = 0, ptr = leaf + 1; int p : pcode) {
        ans[i++] = {p, leaf};
        if (--cnt[p] == 0 && p < ptr) leaf = p;
        else {
            while (cnt[ptr]) ++ptr;
            leaf = ptr++;
        }
    }
    ans.back() = {leaf, n - 1};
    return ans;
}
