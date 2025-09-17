uint64_t kth_root_floor(uint64_t n, size_t k) {
    if (k <= 1) return k ? n : 1;
    if (n <= 1) return n;
    if (k >= 64) return 1;
    auto chk = [&](uint64_t c) {
        uint64_t x = 1;
        for (size_t p = k; p;) {
            if (p & 1) {
                if (x > n / c) return false;
                x *= c;
            }
            if (p >>= 1) {
                if (c > n / c) return false;
                c *= c;
            }
        }
        return x <= n;
    };
    uint64_t r = pow(n, 1.0 / k);
    while (!chk(r)) --r;
    while (chk(r + 1)) ++r;
    return r;
}

uint64_t kth_root_ceil(uint64_t n, size_t k) {
    if (k <= 1) return k ? n : 1;
    if (n <= 1) return n;
    if (k >= 64) return 2;
    auto chk = [&](uint64_t c) {
        uint64_t x = 1;
        for (size_t p = k; p;) {
            if (p & 1) x *= c;
            if (p >>= 1) c *= c;
        }
        return x != n;
    };
    uint64_t r = kth_root_floor(n, k);
    return r + chk(r);
}
