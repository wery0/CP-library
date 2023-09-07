//T - integral type we are working with.
//N - max bitness of numbers, i. e. all numbers are in range [0, 2^N).
//O(N) per insert and count operations, O(N * sizeof(T)) memory.
template<typename T, const size_t N>
class xor_basis {
    static_assert(is_integral_v<T>);
    static_assert(is_unsigned_v<T>);

    array<T, N> store;
    T mask = 0;

public:
    xor_basis() = default;
    template<typename I>
    xor_basis(I first, I last) {
        for (; first != last; ++first) {
            insert(*first);
        }
    }

    int size() const {return __builtin_popcountll(mask);}
    bool empty() const {return mask == 0;}

    void insert(T val) {
        while (val) {
            const int bit = __lg(val);
            if (mask >> bit & 1) val ^= store[bit];
            else {
                store[bit] = val;
                mask ^= (T)1 << bit;
                break;
            }
        }
    }

    bool count(T val) const {
        while (val) {
            const int bit = __lg(val);
            if (mask >> bit & 1) val ^= store[bit];
            else return false;
        }
        return true;
    }

    void merge_with(const xor_basis& other) {
        for (T msk = other.mask; msk; ) {
            const int bit = __lg(msk);
            insert(other.store[bit]);
            msk ^= (T)1 << bit;
        }
    }

    T get_max_possible_xor() const {
        T ans = 0;
        for (T msk = mask; msk; ) {
            const int bit = __lg(msk);
            if (~ans >> bit & 1) ans ^= store[bit];
            msk ^= (T)1 << bit;
        }
        return ans;
    }
};
using basis = xor_basis<int32_t, 30>;
