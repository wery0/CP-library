//constraints: k, m_i >= 0
//decreases sum(m) on min(k, sum(m)), minimizing max remaining element
//returns sorted array after decreasing
template<typename T>
vec<T> remove_k(vec<T> m, T k) {
    int a = m.size();
    sort(all(m));
    ll sm = accumulate(all(m), 0ll);
    ll rem = max(0ll, sm - k);
    ll pr = 0;
    for (int q = 0; q < a; ++q) {
        if (m[q] == pr) continue;
        ll len_suf = a - q;
        ll tyt = len_suf * (m[q] - pr);
        if (tyt <= rem) {
            rem -= tyt;
            pr = m[q];
            continue;
        }
        ll can = pr + rem / len_suf;
        ll eso = rem % len_suf;
        fill(m.begin() + q, m.end() - eso, can);
        fill(m.end() - eso, m.end(), can + 1);
        break;
    }
    return m;
}