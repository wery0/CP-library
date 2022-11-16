template<typename T>
struct closest_pair_of_points_2d {
    using ptt = pair<T, T>;

    ull hash_pair(ull x, ull y) {
        static ull kek = uid<ull>(0, ULLONG_MAX)(rndll);
        return x * kek + y;
    }

    T sqd(ptt p1, ptt p2) {
        return (p1.F - p2.F) * (p1.F - p2.F) + (p1.S - p2.S) * (p1.S - p2.S);
    }

    unordered_map<ull, vector<ptt>> mp;
    vector<ptt> m;

    void rebuild(ld d, int pref) {
        mp.clear();
        for (int q = 0; q < pref; ++q) {
            mp[hash_pair(m[q].F / d, m[q].S / d)].push_back(m[q]);
        }
    }

    closest_pair_of_points_2d() = default;
    closest_pair_of_points_2d(int n) {m.reserve(n);}
    closest_pair_of_points_2d(vector<ptt> m): m(m) {}
    void add_point(T x, T y) {m.emplace_back(x, y);}

    //Expected O(n) with big constant
    pair<ptt, ptt> calc() {
        assert(m.size() >= 2);
        shuffle(all(m), rnd);
        ptt o1 = m[0], o2 = m[1];
        T sd = sqd(o1, o2);
        rebuild(sqrtl(sd), 2);
        for (int q = 2; q < m.size(); ++q) {
            ull x = m[q].F / sqrtl(sd);
            ull y = m[q].S / sqrtl(sd);
            bool fl = 0;
            for (int dx = -1; dx <= 1; ++dx) {
                ull nx = x + dx;
                for (int dy = -1; dy <= 1; ++dy) {
                    ull ny = y + dy;
                    ull hs = hash_pair(nx, ny);
                    if (mp.count(hs)) {
                        for (const ptt& p : mp[hs]) {
                            if (chmin(sd, sqd(m[q], p))) {
                                fl = 1;
                                o1 = m[q], o2 = p;
                                if (sd == 0) return {o1, o2};
                            }
                        }
                    }
                }
            }
            if (fl) rebuild(sqrtl(sd), q + 1);
            else mp[hash_pair(x, y)].push_back(m[q]);
        }
        return {o1, o2};
    }
};