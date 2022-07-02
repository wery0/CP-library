template<typename T>
struct segment_union {
    set<pair<T, T>> s;
    T sz = 0;

    segment_union() {}

    inline void add_seg(T l, T r) {
        auto it = s.upper_bound({l, numeric_limits<T>::min()});
        if (it != s.begin() && (--it)->second < l) ++it;
        while (it != s.end() && it->first <= r) {
            chmin(l, it->F);
            chmax(r, it->S);
            sz -= it->S - it->F + 1;
            it = s.erase(it);
        }
        s.insert({l, r});
        sz += r - l + 1;
    }

    inline T size() {return sz;}

    //returns list of segments of minimal size, which union equals to the union of containing segments
    //{[1, 2], [2, 3], [5, 8], [7, 10]} -> {[1, 3], [5, 10]}
    vec<pair<T, T>> get_segmentaton() {
        if (s.empty()) return {};
        vec<pair<T, T>> m;
        auto [l, r] = *(s.begin());
        for (auto it = next(s.begin()); it != s.end(); ++it) {
            if ((*it).F - 1 == r) r = (*it).S;
            else {
                m.pb({l, r});
                l = (*it).F;
                r = (*it).S;
            }
        }
        m.pb({l, r});
        return m;
    }
};
//Maintains size of union of segments [l, r].