//https://codeforces.com/blog/entry/149516
//Creates a sequence of queries for segment tree beats that supports range add and range chmax which would be processed in O(nlog^2(n))
pair<vector<int>, vector<array<int, 4>>> gen_hack(int n, int q) {
    assert(n);
    auto gen_seq = [&](int n) -> pair<vector<int>, int> {
        vector<int> fib = {0, 1};
        while (fib.size() < n * 2 + 6) fib.push_back(fib.end()[-2] + fib.end()[-1]);
        function<int(int, function<int(int, int)>)> f = [&](int x, function<int(int, int)> g) {
            for (int j = fib.size() - 1; j >= 0; --j) {
                if (j & 1) {
                    if (x == fib[j]) return fib[j + 2];
                    if (x > fib[j]) return fib[j] + g(j, x - fib[j]);
                }
            }
            assert(0);
            return -1;
        };
        function<int(int, int)> g = [&](int j, int x) {
            assert(0 < x && x < fib[j + 1]);
            if (j == 3) return x == 1 ? 2 :  3;
            if (x < fib[j]) return f(x, g);
            if (x == fib[j]) return fib[j + 1];
            if (x > fib[j]) return fib[j] + g(j - 2, x - fib[j]);
            assert(0);
            return -1;
        };
        vector<int> res(fib[n * 2 + 1]);
        for (int i = 0; i < fib[n * 2 + 1]; ++i) res[i] = f(i + 1, g);
        res.back() = fib[n * 2 + 1];
        return {res, fib[n * 2]};
    };
    auto [m, b] = gen_seq(1);
    for (int i = 2; powl(m.size(), 3) < n; ++i) {
        auto [m_, b_] = gen_seq(i);
        m = m_, b = b_;
    }
    int a = m.size();
    for (auto& c : m) c *= -1;
    vector<int> to_chmax;
    for (int i = 0; i < b; ++i) {
        if (m[i] < -b) to_chmax.push_back(m[i]);
    }
    to_chmax.push_back(-b);
    int step = m.size() * m.size();
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        arr[i] = i % step < m.size() ? m[i % m.size()] : -100000;
    }
    vector<array<int, 4>> queries;
    auto add = [&](int L, int R, int l, int r, int x, int sh) {
        assert(L <= l && l <= r && r <= R);
        l += sh, r += sh;
        if (l > R) l -= (R - L + 1);
        if (r > R) r -= (R - L + 1);
        assert(L <= l && l <= R);
        assert(L <= r && r <= R);
        if (l <= r) {
            queries.push_back({0, l, r, x});
        } else {
            queries.push_back({0, l, R, x});
            queries.push_back({0, L, r, x});
        }
    };
    for (int sh = 0; queries.size() < q; sh = (sh + a - b) % m.size()) {
        for (int l = 0; l + m.size() <= arr.size(); l += step) {
            int r = l + m.size() - 1;
            add(l, r, l + b, l + a - 2, a, sh);
        }
        for (int c : to_chmax) {
            queries.push_back({1, 0, int(arr.size()) - 1, c});
        }
        queries.push_back({0, 0, int(arr.size()) - 1, b - a});
    }
    queries.resize(q);
    return {arr, queries};
}
