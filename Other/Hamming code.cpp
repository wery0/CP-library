//Self-correcting code that can fix 1 bit flip
//Uses at most log_2(n) + 2 extra bits
template<typename T>
vector<T> hamming_encode(const vector<T>& m) {
    for (auto& x : m) assert(x == 0 || x == 1); 
    uint64_t extra = 0;
    vector<T> res;
    res.reserve(m.size() + __lg(m.size() + 1) + 2);
    for (size_t i = 0, j = 0; j < m.size(); ++i) {
        res.push_back(i & (i + 1) ? m[j++] : 0);
        if (res[i]) extra ^= i + 1;
    }
    for (size_t i = 0; res.size() >> i; ++i) {
        res[(uint64_t(1) << i) - 1] += extra >> i & 1;
    }
    return res;
}

template<typename T>
vector<T> hamming_decode(const vector<T>& m) {
    for (auto& x : m) assert(x == 0 || x == 1); 
    uint64_t who = 0;
    for (size_t i = 0; i < m.size(); ++i) who ^= m[i] ? i + 1 : 0;
    vector<T> res; res.reserve(m.size());
    for (size_t i = 0; i < m.size(); ++i) {
        if (i & (i + 1)) res.push_back(m[i] ^ (who - 1 == i));
    }
    return res;
}
