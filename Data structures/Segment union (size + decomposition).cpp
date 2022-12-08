template<typename T>
struct segment_union {
    set<pair<T, T>> s;
    T sz = 0;

    segment_union() = default;

    inline void add_seg(T l, T r) {
        auto it = s.upper_bound({l, numeric_limits<T>::min()});
        if (it != s.begin() && (--it)->second < l) ++it;
        while (it != s.end() && it->first <= r) {
            chmin(l, it->first);
            chmax(r, it->second);
            sz -= it->second - it->first + 1;
            it = s.erase(it);
        }
        s.insert({l, r});
        sz += r - l + 1;
    }

    inline T size() {return sz;}

    //Returns list of segments of minimal size, which union equals to the union of containing segments.
    //{[1, 2], [2, 3], [5, 8], [7, 10]} -> {[1, 3], [5, 10]}
    vector<pair<T, T>> get_segmentation() {
        if (s.empty()) return {};
        vector<pair<T, T>> m;
        auto [l, r] = *(s.begin());
        for (auto it = next(s.begin()); it != s.end(); ++it) {
            if ((*it).first - 1 == r) r = (*it).second;
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
//Maintains size of union of segments [l, r].