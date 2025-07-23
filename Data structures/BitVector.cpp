template<typename T, typename C>
class bit_vector {
    static_assert(is_unsigned_v<T>);
    static constexpr int sT = sizeof(T) * 8;
    static constexpr int lgsT = __lg(sT);

    vector<pair<T, C>> m;

    __attribute__((always_inline))
    T allbits(int k) {
        return k == sT ? numeric_limits<T>::max() : (T(1) << k) - 1;
    }

    int popcount(T x) {
        if constexpr(sT <= 32) return __builtin_popcount(x);
        return __builtin_popcountll(x);
    }

public:
    bit_vector() = default;
    bit_vector(int n): m((n + sT - 1) >> lgsT, {0, 0}) {}

    void set(int i) {m[i >> lgsT].first |= T(1) << (i & (sT - 1));}
    int get(int i) {return m[i >> lgsT].first >> (i & (sT - 1)) & 1;}

    void prepare() {
        for (size_t i = 1; i < m.size(); ++i) m[i].second = m[i - 1].second + popcount(m[i - 1].first);
    }

    C pref1(int k) {
        auto [a, b] = m[k >> lgsT];
        return b + popcount(a & allbits((k & (sT - 1)) + 1));
    }

    C pref0(int k) {return k + 1 - pref1(k);}
    C seg0(int l, int r) {return pref0(r) - (l ? pref0(l - 1) : 0);}
    C seg1(int l, int r) {return pref1(r) - (l ? pref1(l - 1) : 0);}
};
