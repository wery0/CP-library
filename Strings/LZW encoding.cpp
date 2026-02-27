//Complexity: <O(n + alphabet), O(n + alphabet)>
template<typename T_arr>
vector<int> lzw_encode(const T_arr& data, const T_arr& alphabet) {
    using T = T_arr::value_type;
    if (data.empty()) return {};
    unordered_map<T_arr, int> mp; mp.reserve(alphabet.size() + data.size());
    for (int i = 0; i < alphabet.size(); ++i) mp[{alphabet[i]}] = i;
    vector<int> encoded;
    T_arr word;
    for (const auto& item : data) {
        word.push_back(item);
        if (!mp.count(word)) {
            mp[word] = mp.size();
            word.pop_back();
            encoded.push_back(mp[word]);
            word = T_arr(1, item);
        }
    }
    encoded.push_back(mp.at(word));
    return encoded;
}

//Complexity: <O(n + alphabet), O(n + alphabet)>
template<typename T_arr>
T_arr lzw_decode(const vector<int>& encoded, const T_arr& alphabet) {
    using T = T_arr::value_type;
    if (encoded.empty()) return {};
    vector<T_arr> mp; mp.reserve(alphabet.size() + encoded.size());
    for (int i = 0; i < alphabet.size(); ++i) mp.push_back({alphabet[i]});
    T_arr decoded = mp[encoded[0]], s = decoded;
    for (int i = 1; i < encoded.size(); ++i) {
        if (encoded[i] < mp.size()) {
            decoded.insert(decoded.end(), mp[encoded[i]].begin(), mp[encoded[i]].end());
            mp.push_back(s); mp.back().push_back(mp[encoded[i]][0]);
            s = mp[encoded[i]];
        } else {
            s.push_back(s[0]);
            decoded.insert(decoded.end(), s.begin(), s.end());
            mp.push_back(s);
        }
    }
    return decoded;
}
