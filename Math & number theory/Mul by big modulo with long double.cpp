//Works for <= 63 bit modulo
uint64_t prod_long_double(const uint64_t x, const uint64_t y, const uint64_t m) {
    uint64_t c = (long double)x * y / m;
    int64_t ans = int64_t(x * y - c * m) % int64_t(m);
    return ans < 0 ? ans + m : ans;
}