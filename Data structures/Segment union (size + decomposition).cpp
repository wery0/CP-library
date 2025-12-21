//Maintains set of minimal size of segments, union of which equals to the content of the data structure
//if is_lattice == true, then segment [l, r] contains numbers l, l + 1, ..., r and T must be integral
//if is_lattice == false, then segment [l, r] is contiguous
/*
     ___________________________________________________________________________________________
    |                    |                                  |                                  |
    |                    |        is_lattice == true        |        is_lattice == false       |
    |____________________|__________________________________|__________________________________|
    |                    |                                  |                                  |
    |  insert_seg(1, 2)  |        {{1, 2}} (len = 2)        |        {{1, 2}} (len = 1)        |
    |____________________|__________________________________|__________________________________|
    |                    |                                  |                                  |
    |  insert_seg(3, 4)  |        {{1, 4}} (len = 4)        |    {{1, 2}, {3, 4}} (len = 2)    |
    |____________________|__________________________________|__________________________________|
    |                    |                                  |                                  |
    |  insert_seg(5, 5)  |        {{1, 5}} (len = 5)        |    {{1, 2}, {3, 4}} (len = 2)    |
    |____________________|__________________________________|__________________________________|
    |                    |                                  |                                  |
    |   erase_seg(3, 3)  |    {{1, 2}, {4, 5}} (len = 4)    |    {{1, 2}, {3, 4}} (len = 2)    |
    |____________________|__________________________________|__________________________________|
    |                    |                                  |                                  |
    |   erase_seg(2, 3)  |    {{1, 1}, {4, 5}} (len = 3)    |    {{1, 2}, {3, 4}} (len = 2)    |
    |____________________|__________________________________|__________________________________|
    |                    |                                  |                                  |
    |   erase_seg(2, 4)  |    {{1, 1}, {5, 5}} (len = 2)    |        {{1, 2}} (len = 1)        |
    |____________________|__________________________________|__________________________________|
*/
template<typename T, const bool is_lattice = true>
class segment_union {
    static_assert(is_integral_v<T> || !is_lattice);
    set<pair<T, T>> s;
    T union_length = 0;

public:
    segment_union() = default;

    void clear() {s.clear(); union_length = 0;}

    void insert_seg(T l, T r) {
        assert(l <= r);
        if (!is_lattice && l == r) return;
        auto it = s.upper_bound({l, numeric_limits<T>::min()});
        if (it != s.begin() && (--it)->second < l - is_lattice) ++it;
        while (it != s.end() && it->first <= r + is_lattice) {
            l = min(l, it->first);
            r = max(r, it->second);
            union_length -= it->second - it->first + is_lattice;
            it = s.erase(it);
        }
        s.emplace(l, r);
        union_length += r - l + is_lattice;
    }

    void erase_seg(T l, T r) {
        assert(l <= r);
        if (!is_lattice && l == r) return;
        insert_seg(l, r);
        auto it = prev(s.upper_bound({l + 1, l}));
        auto [sl, sr] = *it;
        s.erase(it);
        union_length -= sr - sl + is_lattice;
        if (sl < l) s.emplace(sl, l - is_lattice), union_length += l - sl;
        if (sr > r) s.emplace(r + is_lattice, sr), union_length += sr - r;
    }

    //If is_lattice == true returns # of integral points in union of all segments
    //If is_lattice == false returns length of union of all segments
    T get_union_length() const {return union_length;}

    vector<pair<T, T>> get_segmentation() const {
        return vector<pair<T, T>>(s.begin(), s.end());
    }

    //Returns {m1, m2} where m1 = lhs - rhs, i. e. a vector of segments that are present in lhs (== this) but not present in rhs. m2 = rhs - lhs.
    array<vector<pair<T, T>>, 2> get_diff_stupid(const segment_union<T, is_lattice>& rhs) const {
        auto cp1 = *this, cp2 = rhs;
        for (auto [l, r] : rhs.get_segmentation()) cp1.erase_seg(l, r); 
        for (auto [l, r] : get_segmentation()) cp2.erase_seg(l, r);
        return {cp1.get_segmentation(), cp2.get_segmentation()}; 
    }
};
