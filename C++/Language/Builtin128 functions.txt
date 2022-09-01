int __builtin_popcount128(__uint128_t x) {return __builtin_popcountll(x) + __builtin_popcountll(x >> 64);}
int __lg128(__uint128_t x) {return (x >> 64) ? 64 + __lg((ull)(x >> 64)) : __lg((ull)x);}
int __builtin_ctz128(__uint128_t x) {return (x << 64) ? __builtin_ctzll((ull)x) : 64 + __builtin_ctzll((ull)(x >> 64));}
int __builtin_clz128(__uint128_t x) {return (x >> 64) ? __builtin_clzll((ull)(x >> 64)) : 64 + __builtin_clzll((ull)x);}
int __builtin_parity128(__uint128_t x) {return __builtin_parityll((ull)x) ^ __builtin_parityll((ull)(x >> 64));}
