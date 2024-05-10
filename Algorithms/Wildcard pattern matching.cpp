// Given text and pattern over some alphabet and a wildcard character (wc), which can be mapped to any character
// Returns binary string res of length |text| - |pattern| + 1, s. t. res[i] == '1' if text[i : i + |text|] == pattern
// Chance of res[i] being false-positive (1 instead of 0) is 1 / mint::get_mod()
// O(nlog(n))
/*
Example:
   text = a*ac*ba
pattern = *c*
f(text, pattern, '*') = "10110"
*/
template<typename T_arr, typename T, typename mint>
string wildcard_pattern_matching(const T_arr& text, const T_arr& pattern, T wc) {
    using C = uint64_t;
    static uniform_int_distribution<C> gen(1, numeric_limits<C>::max());
    static mt19937 rng;
    const size_t N = text.size(), M = pattern.size();
    if (N == 0 || M > N) return "";
    assert(M);
    map<T, C> char_to_rnd;
    auto f = [&](T x) {auto& res = char_to_rnd[x]; if (!res) res = gen(rng); return res;};
    vector<mint> mt(N), mp(M);
    for (size_t i = 0; T x : text) mt[i++] = x == wc ? 0 : f(x);
    for (size_t i = M; T x : pattern) mp[--i] = x == wc ? 0 : f(x);
    vector<mint> sc1 = sum_convolution<mint>(mp, mt);
    for (size_t i = 0; T x : text) mt[i++] = x == wc;
    for (mint& x : mp) x *= x;
    mint must = accumulate(mp.begin(), mp.end(), mint(0));
    vector<mint> sc2 = sum_convolution<mint>(mp, mt);
    string res(N - M + 1, '0');
    for (size_t i = M - 1; i < N; ++i) res[i + 1 - M] += sc1[i] + sc2[i] == must;
    return res;
}
