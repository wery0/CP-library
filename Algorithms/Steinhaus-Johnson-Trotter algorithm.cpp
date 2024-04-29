//Steinhaus–Johnson–Trotter algorithm for generating hamiltonian cycle in permutohedron
template<typename T>
vector<vector<T>> sjt(T n) {
    assert(n >= 0);
    if (n == 0) return {{}};
    vector<vector<T>> res;
    for (int u = 0; auto p : sjt(n - 1)) {
        for (int i = 0; i < n; ++i) {
            size_t pos = u ? i : n - 1 - i;
            p.insert(p.begin() + pos, n);
            res.push_back(p);
            p.erase(p.begin() + pos);
        }
        u ^= 1;
    }
    return res;
}

template<typename T>
vector<T> kth_sjt(T n, T k) {
    assert(n >= 0);
    vector<T> p(n);
    auto find_kth_0 = [&](T k) {
        for (size_t i = 0; i < n; ++i) {
            if ((k -= p[i] == 0) == 0) return i;
        }
        assert(0);
    };
    for (T u = n; u; k /= u, --u) {
        T rem = k % (u * 2);
        T posn = rem < u ? u - rem : rem + 1 - u;
        p[find_kth_0(posn)] = u;
    }
    return p;
}
