template<typename T>
class pairwise_fp_summator {
    static_assert(is_floating_point_v<T>);

    size_t sz = 0;
    vector<T> m;

public:
    pairwise_fp_summator() = default;

    void clear() {
        fill(m.begin(), m.end(), 0);
        sz = 0;
    }

    //O(1) amortized, O(log(sz)) worst case
    void add(T x) {
        if (!(sz & (++sz))) m.push_back(0);
        m[0] += x;
        for (size_t u = 0; ~sz >> u & 1; ++u) {
            m[u + 1] += m[u];
            m[u] = 0;
        }
    }

    //O(log(sz))
    T get_sum() {
        return accumulate(m.begin(), m.end(), T(0));
    }
};
