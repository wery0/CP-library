//Nicely estimates number of distinct elements ih array/stream. Easily mergeable.
//https://en.wikipedia.org/wiki/HyperLogLog
//Increasing p = better estimate, more memory usage, slower estimating and merging time
//Memory usage: 2 ^ p + 64 * 8 + 4 = O(2 ^ p) bytes
template<typename T, int p = 12>
class HyperLogLog {

    static constexpr uint64_t M = uint64_t(1) << p;
    static constexpr long double alpha = M == 16 ? 0.673 : M == 32 ? 0.697 : M == 64 ? 0.709 : 0.7213 / (1 + 1.079 / M);
    static constexpr long double C = alpha * M * M;
    size_t added = 0;
    array<uint8_t, M> m{0};
    array<int64_t, 64 - p + 2> cnt{0};

    static uint8_t rank(uint64_t x) {return min(64 - p + 1, __builtin_ctzll(x) + 1);}

    static uint64_t hsh(uint64_t x) {
        x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
        x = x ^ (x >> 31);
        return x;
    }

    static uint64_t hsh(string x) {
        uint64_t hs = 0, c = 666667;
        for (uint64_t pw = 1; char c : x) {
            hs += c * pw;
            pw *= c;
        }
        return hs;
    }

    static uint64_t calc_hash(const T& xx) {
        uint64_t x;
        if constexpr(is_integral_v<T>) x = xx;
        else x = hsh(xx);
        return hsh(x);
    }

    static pair<uint64_t, uint8_t> calc(const T& x) {
        uint64_t hs = calc_hash(x);
        return {hs & (M - 1), rank(hs >> p)};
    }

public:
    HyperLogLog() {
        cnt[0] = M;
    }

    void inplace_merge_with(const HyperLogLog& rhs) {
        added += rhs.added;
        fill(cnt.begin(), cnt.end(), 0);
        for (size_t i = 0; i < m.size(); ++i) {
            //TODO: Use _mm512_max_epi8
            m[i] = max(m[i], rhs.m[i]);
            ++cnt[m[i]];
        }
    }

    void clear() {
        added = 0;
        fill(cnt.begin(), cnt.end(), 0);
        cnt[0] = M;
        fill(m.begin(), m.end(), 0);
    }

    void insert(const T& x) {
        ++added;
        auto [i, rank] = calc(x);
        if (rank > m[i]) {
            --cnt[m[i]];
            ++cnt[rank];
            m[i] = rank;
        }
    }

    //O(cnt.size() == 64 - p) for the first way, O(m.size() == M == 2 ^ p) for the second way
    size_t estimate_distinct() const {
        long double sum = 0;
        for (size_t i = 0; i < cnt.size(); ++i) sum += (long double)(cnt[i]) / (uint64_t(1) << i);
        // long double sum = 0;
        // for (auto i : m) sum += (long double)(1) / (uint64_t(1) << i); 
        size_t res = size_t(roundl(C / sum));
        res = min(res, added);
        return res;
    }
};
