//Sorts an array of 32/64 bit integers using radix sort
//Set parameter bit to 32 if you wanna sort numbers based on its first 32 bits
template<typename T, int bit = 0>
void radix_sort(T* p, size_t n) {
    static_assert(sizeof(T) == 4 || sizeof(T) == 8);
    static_assert(bit == 0 || bit == 32 || bit == 64);
    static_assert(bit <= sizeof(T) * 8);
    static constexpr uint32_t b = 8;
    static constexpr uint32_t powb = 1 << b;
    static constexpr uint32_t mask = powb - 1;
    static uint32_t cnt0[powb];
    static uint32_t cnt1[powb];
    static uint32_t cnt2[powb];
    static uint32_t cnt3[powb];
    if (n <= 64) {
        if constexpr(sizeof(T) == 8 && bit == 32) {
            sort(p, p + n, [](T x, T y) {return (x & UINT32_MAX) < (y & UINT32_MAX);});
        } else {
            sort(p, p + n);
        }
        return;
    }
    static T* tmp = 0;
    static size_t tmp_size = 0;
    if (tmp_size < n) {
        delete[] tmp;
        tmp_size = 2 << __lg(n);
        tmp = new T[tmp_size];
    }
    memset(cnt0, 0, sizeof(cnt0));
    memset(cnt1, 0, sizeof(cnt1));
    memset(cnt2, 0, sizeof(cnt2));
    memset(cnt3, 0, sizeof(cnt3));
    for (size_t i = 0; i < n; ++i) {
        ++cnt0[p[i] & mask];
        ++cnt1[(p[i] >> b) & mask];
        ++cnt2[(p[i] >> b * 2) & mask];
        ++cnt3[(p[i] >> b * 3) & mask];
    }
    for (size_t i = 0; i < powb - 1; ++i) {
        cnt0[i + 1] += cnt0[i];
        cnt1[i + 1] += cnt1[i];
        cnt2[i + 1] += cnt2[i];
        cnt3[i + 1] += cnt3[i];
    }
    for (size_t i = n; i--;) tmp[--cnt0[p[i] & mask]] = p[i];
    for (size_t i = n; i--;) p[--cnt1[tmp[i] >> b & mask]] = tmp[i];
    for (size_t i = n; i--;) tmp[--cnt2[p[i] >> b * 2 & mask]] = p[i];
    for (size_t i = n; i--;) p[--cnt3[tmp[i] >> b * 3 & mask]] = tmp[i];
    if constexpr (sizeof(T) == 8 && bit != 32) {
        memset(cnt0, 0, sizeof(cnt0));
        for (int i = 0; i < n; ++i) cnt0[p[i] >> b * 4 & mask]++;
        for (int i = 0; i < powb - 1; ++i) cnt0[i + 1] += cnt0[i];
        for (int i = n; i--;) tmp[--cnt0[p[i] >> b * 4 & mask]] = p[i];
        memset(cnt0, 0, sizeof(cnt0));
        for (int i = 0; i < n; ++i) cnt0[tmp[i] >> b * 5 & mask]++;
        for (int i = 0; i < powb - 1; ++i) cnt0[i + 1] += cnt0[i];
        for (int i = n; i--;) p[--cnt0[tmp[i] >> b * 5 & mask]] = tmp[i];
        memset(cnt0, 0, sizeof(cnt0));
        for (int i = 0; i < n; ++i) cnt0[p[i] >> b * 6 & mask]++;
        for (int i = 0; i < powb - 1; ++i) cnt0[i + 1] += cnt0[i];
        for (int i = n; i--;) tmp[--cnt0[p[i] >> b * 6 & mask]] = p[i];
        memset(cnt0, 0, sizeof(cnt0));
        for (int i = 0; i < n; ++i) cnt0[tmp[i] >> b * 7 & mask]++;
        for (int i = 0; i < powb - 1; ++i) cnt0[i + 1] += cnt0[i];
        for (int i = n; i--;) p[--cnt0[tmp[i] >> b * 7 & mask]] = tmp[i];
    }
    if constexpr (is_signed<T>::value) {
        int i = n;
        while (i && p[i - 1] < 0) i--;
        rotate(p, p + i, p + n);
    }
}
