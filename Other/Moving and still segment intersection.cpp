/*
  Describes intersection of moving segment [l, r] with still segment [L, R] at all shifts where intersection is positive as
  a vector of [pl, pr, a, b] which defines arithmetic progression a + bi being added to segment [pl, pr]. To get the length
  of intersection at point i, sum the values of all progressions at point i.
  Correctness: stress-tested
  Example:
  l = 5, r = 10, L = 3, R = 7
  f(l, r, L, R) = {
    {-6, -4, 1, 1},
    {-3, -3, 4, 0},
    {-2, 1, 4, -1}
  }
*/
template<typename T>
vector<array<T, 4>> moving_and_still_segment_intersection(T l, T r, const T L, const T R, T min_shift = 1, T max_shift = 0) {
    if (r - l <= 0 || R - L <= 0) return {};
    vector<array<T, 4>> res;
    T mnd = min(R - L, r - l), sh = L + 1 - r;
    l += sh, r += sh;
    if (T eso = min(R - r, L - l); eso > 0) {
        res.push_back({sh, sh + eso - 1, 1, 1});
        sh += eso, l += eso, r += eso;
    }
    if (T eso = max(R - r, L - l); eso > 0) {
        res.push_back({sh, sh + eso - 1, mnd, 0});
        sh += eso;
    }
    res.push_back({sh, sh + mnd - 1, mnd, -1});
    if (min_shift <= max_shift) {
        vector<array<T, 4>> new_res;
        for (auto [pl, pr, st, d] : res) {
            st += max(min_shift - pl, T(0)) * d;
            pl = max(pl, min_shift);
            pr = min(pr, max_shift);
            if (pl <= pr) new_res.push_back({pl, pr, st, d});
        }
        res = new_res;
    }
    return res;
}
