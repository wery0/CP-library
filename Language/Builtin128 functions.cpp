int __builtin_popcount128(__uint128_t x) {return __builtin_popcountll(x) + __builtin_popcountll(x >> 64);}
int __lg128(__uint128_t x) {return (x >> 64) ? 64 + __lg(uint64_t(x >> 64)) : __lg(uint64_t(x));}
int __builtin_ctz128(__uint128_t x) {return (x << 64) ? __builtin_ctzll(uint64_t(x)) : 64 + __builtin_ctzll(uint64_t(x >> 64));}
int __builtin_clz128(__uint128_t x) {return (x >> 64) ? __builtin_clzll(uint64_t(x >> 64)) : 64 + __builtin_clzll(uint64_t(x));}
int __builtin_parity128(__uint128_t x) {return __builtin_parityll(uint64_t(x)) ^ __builtin_parityll(uint64_t(x >> 64));}
