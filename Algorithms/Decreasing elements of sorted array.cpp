//Constraints: k, m_i >= 0
//Decreases sum(m) on min(k, sum(m)), minimizing max remaining element
//Returns sorted array after decreasing
template<typename T>
vector<T> remove_k(vector<T> m, T k) {
    const int n = m.size();
    sort(m.begin(), m.end());
    ll sm = accumulate(m.begin(), m.end(), 0ll);
    ll rem = max(0ll, sm - k);
    ll pr = 0;
    for (int i = 0; i < n; ++i) {
        if (m[i] == pr) continue;
        ll len_suf = n - i;
        ll tyt = len_suf * (m[i] - pr);
        if (tyt <= rem) {
            rem -= tyt;
            pr = m[i];
            continue;
        }
        ll can = pr + rem / len_suf;
        ll eso = rem % len_suf;
        fill(m.begin() + i, m.end() - eso, can);
        fill(m.end() - eso, m.end(), can + 1);
        break;
    }
    return m;
}