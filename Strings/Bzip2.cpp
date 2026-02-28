string good_chars() {
    string res, bad = "\"\\?";
    for (int i = 32; i < 127; ++i) {
        char c = i;
        if (!count(bad.begin(), bad.end(), c)) res += char(i);
    }
    return res;
}

template<class T> struct is_pair: std::false_type {};
template<class T1, class T2> struct is_pair<std::pair<T1, T2>>: std::true_type {};
template<class T> struct is_pair_d: is_pair<typename std::decay<T>::type> {};
template<typename T>
string stringify(const T& x) {
    if constexpr (is_same_v<T, char>) return "\'" + string(1, x) + "\'";
    else if constexpr (is_arithmetic_v<T> || is_same_v<T, __int128_t> || is_same_v<T, __uint128_t>) return to_string(x);
    else if constexpr (is_same_v<T, std::string>) return "\"" + x + "\"";
    else if constexpr (is_pair_d<T>::value) return "{" + stringify(x.first) + "," + stringify(x.second) + "}";
    else {
        string res = "{";
        for (int i = 0; const auto& c : x) res += (i++ ? "," : "") + stringify(c);
        return res + "}";
    }
}

//Implementation of bzip2 compression algorithm
//Complexity: <O(nlog(n) + (nlog(2) / log(92) + d) * n / d), O(n)>
template<typename T_arr>
string bzip2_encode(T_arr data, int d) {
    using T = T_arr::value_type;
    string gc = good_chars();
    auto [e1, h1] = bwt_encode<T_arr>(data);
    auto [e2, h2] = mtf_encode<T_arr>(e1);
    auto [e3, h3] = huffman_encode<vector<int>>(e2);
    vector<int> ae3(e3.size());
    for (int i = 0; i < e3.size(); ++i) ae3[i] = e3[i] - '0';
    vector<int> ae4 = encode<int>(gc.size(), {2}, {ae3}, d);
    string e4(ae4.size(), '?');
    int h4 = e3.size();
    for (int i = 0; i < e4.size(); ++i) e4[i] = gc[ae4[i]];
    string compressed = "T_arr decoded = bzip2_decode<T_arr>(" + stringify(e4) + ", " + stringify(h1) + ", " + stringify(h2) + ", " + stringify(h3) + ", " + stringify(h4) + ", " + stringify(d) + ");";
    return compressed;
}

template<typename T_arr>
T_arr bzip2_decode(string e4, int h1, vector<typename T_arr::value_type> h2, map<string, int> h3, int h4, int d) {
    using T = T_arr::value_type;
    string gc = good_chars();
    int mpp[256]; for (int i = 0; i < gc.size(); ++i) mpp[gc[i]] = i;
    vector<int> ae4(e4.size()); for (int i = 0; i < e4.size(); ++i) ae4[i] = mpp[e4[i]];
    vector<int> ae3 = decode<int>(gc.size(), {2}, {h4}, {ae4}, d)[0];
    string e3(ae3.size(), '0');
    for (int i = 0; i < e3.size(); ++i) e3[i] += ae3[i];
    auto e2 = huffman_decode<vector<int>>(e3, h3);
    auto e1 = mtf_decode<T_arr>(e2, h2);
    T_arr data = bwt_decode<T_arr>(e1, h1);
    return data;
}
