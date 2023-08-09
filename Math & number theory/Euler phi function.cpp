uint64_t euler_phi_function(uint64_t n) {
    uint64_t o = 1, p = 1, pr = 4;
    for (uint64_t x : factorize(n)) {
        if (x == pr) p *= x;
        else {
            o *= p - p / pr;
            p = pr = x;
        }
    }
    o *= p - p / pr;
    return o;
}
