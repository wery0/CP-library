//Maintains union of segments [l, r]
//if is_lattice == true, then segment [l, r] contains numbers l, l + 1, ..., r and T must be integral
//if is_lattice == false, then segment [l, r] is contiguous
template<typename T, const bool is_lattice = true>
class segment_union {
    static_assert(is_integral_v<T> || !is_lattice);
    set<pair<T, T>> s;
    T sz = 0;

public:
    segment_union() = default;

    void clear() {s.clear(); sz = 0;}

    void add_seg(T l, T r) {
        assert(l <= r);
        if (!is_lattice && l == r) return;
        auto it = s.upper_bound({l, numeric_limits<T>::min()});
        if (it != s.begin() && (--it)->second < l) ++it;
        while (it != s.end() && it->first <= r) {
            l = min(l, it->first);
            r = max(r, it->second);
            sz -= it->second - it->first + is_lattice;
            it = s.erase(it);
        }
        s.emplace(l, r);
        sz += r - l + is_lattice;
    }

    void erase_seg(T l, T r) {
        assert(l <= r);
        if (!is_lattice && l == r) return;
        add_seg(l, r);
        auto it = prev(s.upper_bound({l + 1, l}));
        auto [sl, sr] = *it;
        s.erase(it);
        sz -= sr - sl + is_lattice;
        if (sl < l) s.emplace(sl, l - is_lattice), sz += l - sl;
        if (sr > r) s.emplace(r + is_lattice, sr), sz += sr - r;
    }

    //If is_lattice == true returns # numbers in union of all segments
    //If is_lattice == false returns length of union of all segments
    T size() const {return sz;}

    //Returns list of segments of minimal size, which union equals to the union of containing segments
    //{[1, 2], [2, 3], [5, 8], [7, 10], [12, 13], [14, 15]} -> {[1, 3], [5, 10], [12, 15]} if is_lattice == true
    //{[1, 2], [2, 3], [5, 8], [7, 10], [12, 13], [14, 15]} -> {[1, 3], [5, 10], [12, 13], [14, 15]} if is_lattice == false
    vector<pair<T, T>> get_segmentation() const {
        if (s.empty()) return {};
        vector<pair<T, T>> m;
        auto [l, r] = *s.begin();
        for (auto it = next(s.begin()); it != s.end(); ++it) {
            if ((*it).first - is_lattice == r) r = (*it).second;
            else {
                m.emplace_back(l, r);
                l = (*it).first;
                r = (*it).second;
            }
        }
        m.emplace_back(l, r);
        return m;
    }
};