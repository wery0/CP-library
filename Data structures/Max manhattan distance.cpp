//T - type of point coordinates
//D - number of dimensions
template<typename T, const size_t D>
class MaxManhattanDistance {
    static_assert(is_signed_v<T>);
    static_assert(D > 0);

    static constexpr size_t K = size_t(1) << (D - 1);

    using point = array<T, D>;

    multiset<point> points;
    array<multiset<T>, K> sums;

    T calc_sum(const point& p, size_t mask) const {
        T s = p[0];
        for (size_t i = 1; i < D; ++i) {
            s += mask >> (i - 1) & 1 ? p[i] : -p[i];
        }
        return s;
    }

    T eval_sums(size_t i) const {
        return *prev(sums[i].end()) - *sums[i].begin();
    }

public:
    MaxManhattanDistance() = default;
    template<typename I>
    MaxManhattanDistance(I first, I last) {
        for (auto i = first; i != last; ++i) {
            insert_point(*i);
        }
    }

    size_t size() const {return sums[0].size();}
    void clear() {for (auto& s : sums) s.clear();}
    bool contains(const point& p) const {return points.find(p) != points.end();}

    //<O(Klog(n)), O(K)>
    void insert_point(const point& p) {
        points.insert(p);
        for (size_t mask = 0; mask < K; ++mask) {
            sums[mask].insert(calc_sum(p, mask));
        }
    }

    //O(Klog(n))
    void erase_point(const point& p) {
        if (!contains(p)) return;  //Point must exist to perform deletion of sums
        points.erase(points.find(p));
        for (size_t mask = 0; mask < K; ++mask) {
            sums[mask].erase(sums[mask].find(calc_sum(p, mask)));
        }
    }

    //O(K)
    T calc_max_manhattan_distance() const {
        assert(size());
        T res = 0;
        for (size_t mask = 0; mask < K; ++mask) {
            res = max(res, eval_sums(mask));
        }
        return res;
    }
};
