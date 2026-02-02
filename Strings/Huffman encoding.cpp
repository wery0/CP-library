//Complexity: <O(nlog(distinct characters)), O(n)>
template<typename T_arr>
pair<string, map<string, typename T_arr::value_type>> huffman_encode(T_arr data) {
    using T = T_arr::value_type;
    if (data.empty()) return {"", {}};
    map<T, int> mp;
    for (const auto& item : data) ++mp[item];
    const int n = mp.size();
    vector<pair<T, int>> vmp(mp.begin(), mp.end());
    vector<array<int, 2>> chd(n - 1);
    priority_queue<pair<int, int>> pq;
    for (int i = 0; i < n; ++i) pq.emplace(-vmp[i].second, n - 1 + i);
    for (int i = 0; i < n - 1; ++i) {
        auto [c1, v1] = pq.top(); pq.pop();
        auto [c2, v2] = pq.top(); pq.pop();
        chd[i] = {v1, v2};
        pq.emplace(c1 + c2, i);
    }
    int root = pq.top().second;
    map<T, string> item_to_code;
    map<string, T> code_to_item;
    function<void(int, string)> dfs = [&](int v, string code) {
        if (v > n - 2) {
            if (code.size() == 0) code = "0";
            item_to_code[vmp[v + 1 - n].first] = code;
            code_to_item[code] = vmp[v + 1 - n].first;
            return;
        }
        dfs(chd[v][0], code + "0");
        dfs(chd[v][1], code + "1");
    };
    dfs(root, "");
    string encoded;
    for (const auto& item : data) encoded += item_to_code[item];
    return {encoded, code_to_item};
}

//Complexity: <O(n), O(n)>
template<typename T_arr>
T_arr huffman_decode(string encoded, map<string, typename T_arr::value_type> code_to_item) {
    if (encoded.empty()) return T_arr();
    const int n = code_to_item.size();
    vector<array<int, 2>> chd(1, {-1, -1});
    for (auto [code, item] : code_to_item) {
        for (int v = 0; char c : code) {
            int dir = c == '1';
            if (chd[v][dir] == -1) {
                chd[v][dir] = chd.size();
                chd.push_back({-1, -1});
            }
            v = chd[v][dir];
        }
    }
    T_arr decoded;
    string code;
    for (int v = 0; char c : encoded) {
        v = chd[v][c == '1'];
        code += c;
        if (chd[v][0] == -1 && chd[v][1] == -1) {
            decoded.push_back(code_to_item.at(code));
            code = "";
            v = 0;
        }
    }
    return decoded;
}
