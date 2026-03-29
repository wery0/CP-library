//Implementation of induced sorting algorithm for calculating suffix array
//Complexity: O(nlog(n)) for initial sorting and value compression (optimizable for small alphabets) and O(n) for finding suffix array afterwards
template<typename T_arr>
vector<int> suffix_array_induced_sorting(T_arr& s, bool is_cyclic) {
    function<vector<int>(const vector<int>&, int)> go = [&](const vector<int>& s, int K) {
        const int n = s.size();
        vector<char> t(n);
        vector<int> bkt(K, 0), bkt_l(K), bkt_r(K), sa(n), p1;
        t.back() = 1;
        for (int i = n - 2; i >= 0; --i) {
            t[i] = s[i] < s[i + 1] || t[i + 1] && s[i] == s[i + 1];
            if (!t[i] && t[i + 1]) p1.push_back(i + 1);
        }
        reverse(p1.begin(), p1.end());
        const int n1 = p1.size();
        for (int i = 0; i != n; ++i) ++bkt[s[i]];
        for (int i = 0, sum = 0; i != K; ++i) bkt_l[i] = sum, bkt_r[i] = (sum += bkt[i]);
        vector<int> s1(n1), sa1(n1);
        fill_n(sa.begin(), n, -1);
        copy_n(bkt_r.begin(), K, bkt.begin());
        for (int i = n1 - 1; i >= 0; --i) sa[--bkt[s[p1[i]]]] = p1[i];
        copy_n(bkt_l.begin(), K, bkt.begin());
        for (int i = 0, j; i != n; ++i) if ((j = sa[i] - 1) >= 0 && !t[j]) sa[bkt[s[j]]++] = j;
        copy_n(bkt_r.begin(), K, bkt.begin());
        for (int i = n - 1, j; i >= 0; --i) if ((j = sa[i] - 1) >= 0 && t[j]) sa[--bkt[s[j]]] = j;
        for (int i = 0, j = 0; i != n; ++i) if (sa[i] && !t[sa[i] - 1] && t[sa[i]]) sa1[j++] = sa[i];
        int name = 0;
        for (int i = 0, prev = -1; i != n1; ++i) {
            int pos = sa1[i];
            for (int j = 0; ; ++j) {
                if (prev == -1 || s[pos + j] != s[prev + j] || t[pos + j] != t[prev + j]) {
                    prev = pos, ++name;
                    break;
                } else if (j != 0 && (pos + j && !t[pos + j - 1] && t[pos + j] || prev + j && !t[prev + j - 1] && t[prev + j])) break;
            }
            sa[pos] = name - 1;
        }
        for (int i = 0; i != n1; ++i) s1[i] = sa[p1[i]];
        if (name != n1) sa1 = go(s1, name);
        else for (int i = 0; i != n1; ++i) sa1[s1[i]] = i;
        copy_n(bkt_r.begin(), K, bkt.begin());
        fill_n(sa.begin(), n, -1);
        for (int i = n1 - 1; i >= 0; --i) sa[--bkt[s[p1[sa1[i]]]]] = p1[sa1[i]];
        for (int i = 0, j; i != n; ++i) if ((j = sa[i] - 1) >= 0 && !t[j]) sa[bkt_l[s[j]]++] = j;
        for (int i = n - 1, j; i >= 0; --i) if ((j = sa[i] - 1) >= 0 && t[j]) sa[--bkt_r[s[j]]] = j;
        return sa;
    };
    using T = T_arr::value_type;
    assert(all_of(s.begin(), s.end(), [](const auto& i) {return i > 0;}));
    if (is_cyclic) s.insert(s.end(), s.begin(), s.end());
    s.push_back(0);  //This symbol must not appear in the sequence and must be the smallest one
    vector<T> df(s.begin() + (is_cyclic ? s.size() / 2 : 0), s.end());
    sort(df.begin(), df.end());
    df.erase(unique(df.begin(), df.end()), df.end());
    vector<int> s_cpy(s.size());
    for (size_t i = is_cyclic ? s.size() / 2 : 0; i < s.size(); ++i) s_cpy[i] = lower_bound(df.begin(), df.end(), s[i]) - df.begin();
    if (is_cyclic) copy_n(s_cpy.begin() + s.size() / 2, s.size() / 2, s_cpy.begin());
    vector<int> sa = go(s_cpy, df.size());
    if (is_cyclic) {
        sa.erase(remove_if(sa.begin(), sa.end(), [&](int i) {return i >= s.size() / 2;}), sa.end());
    } else {
        sa.erase(sa.begin());
    }
    return sa;
}
