//Faster and more secure random number generator compared to mt19937
//https://codeforces.com/blog/entry/153335?#comment-1362351
#include <immintrin.h>
class AesCtrRng {
    uint8_t p;
    alignas(16) uint64_t v[2];
    uint64_t c0, c1;
    __m128i key1, key2;

public:
    __attribute__((__target__("rdrnd")))
    AesCtrRng(uint64_t seed = 0): p(1) {
        // _rdrand64_step(&seed);    //Uncomment this line if you want completely random seed
        alignas(16) uint64_t sd[6];
        mt19937 rnd(seed); for (int i = 0; i < 6; ++i) sd[i] = rnd();
        c0 = sd[0], c1 = sd[1];
        key1 = _mm_set_epi64x(sd[2], sd[3]);
        key2 = _mm_set_epi64x(sd[4], sd[5]);
    }

    __attribute__((__target__("aes")))
    uint64_t operator()() {
        if (p) {
            if (++c0 == 0) ++c1;
            __m128i state = _mm_set_epi64x(c1, c0);
            state = _mm_aesenc_si128(state, key1);
            state = _mm_aesenc_si128(state, key2);
            state = _mm_aesenc_si128(state, key1);
            _mm_store_si128((__m128i*)v, state);
        }
        return v[p ^= 1];
    }
};
