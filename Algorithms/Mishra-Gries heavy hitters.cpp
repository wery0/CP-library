//Streaming algorithm for finding all elements that occur > n / k times
//Returns vector of pairs {element, occurrences}
//<O(n), O(k)> 
template<typename T>
vector<pair<T, int>> mishra_gries_heavy_hitters(auto first, auto last, size_t k) {
    assert(k >= 2);
    const size_t n = std::distance(first, last);
    umap<T, int> mp(k * 2);
    size_t reduces = 0;
    for (auto it = first; it != last; ++it) {
        ++mp[*it];
        if (mp.size() == k) {
            for (auto& [c, cnt] : mp) --cnt;
            erase_if(mp, [](const auto& p) {return p.second == 0;});
            ++reduces;
        }
    }
    vector<pair<T, int>> res(mp.size());
    for (size_t i = 0; const auto& [c, cnt] : mp) {
        res[i++] = {c, cnt + reduces};
    }
    return res;
}
