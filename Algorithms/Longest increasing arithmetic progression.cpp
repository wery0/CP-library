template<typename T>
struct longest_increasing_arithmetic_progression {

    template<typename K, typename V, const int LG>
    struct hash_map_open_addressing {

        const int N = 1 << LG;
        vector<pair<K, V>> store = vector<pair<K, V>>(N);
        vector<bool> is_occupied = vector<bool>(N);

        constexpr inline void next_pos(int& pos) const {
            pos = pos < N - 1 ? pos + 1 : 0;
        }

        static constexpr uint64_t kek = 11995408973635179863ull;
        constexpr inline int hsh(const K& key) const {
            if constexpr(is_integral_v<K>) {
                //return key & (N - 1);
                return (key * kek) >> (64 - LG);
            } else {
                return hash<K>{}(key) % N;
            }
        }

        void clear() {
            fill(is_occupied.begin(), is_occupied.end(), false);
        }

        V operator[](const K& key) const {
            int pos = hsh(key);
            while (is_occupied[pos]) {
                if (store[pos].first == key) return store[pos].second;
                next_pos(pos);
            }
            return -1;
        }

        void add(const K& key, const V& val) {
            int pos = hsh(key);
            while (is_occupied[pos]) {
                if (store[pos].first == key) {
                    store[pos].second = val;
                    return;
                }
                next_pos(pos);
            }
            store[pos] = {key, val};
            is_occupied[pos] = true;
        }
    };

    //Returns positions of elements of longest increasing(!) arithmetic progression
    //https://acm.timus.ru/problem.aspx?space=1&num=1395
    //O(n ^ 3), but very optimized
    hash_map_open_addressing<T, int, 17> mp; //change, if need
    vector<int> solve(vector<T> m) {
        size_t a = m.size(), na = 0;
        vector<pair<T, int>> stm(a);
        vector<int> ps(a);
        for (size_t i = 0; i < a; ++i) stm[i] = {m[i], i};
        sort(stm.begin(), stm.end());
        mp.clear();
        for (size_t i = 0; i < a; ++i) {
            if (i && stm[i].first == stm[i - 1].first) continue;
            m[na] = stm[i].first;
            mp.add(m[na], na);
            ps[na++] = stm[i].second;
        }
        int o = 1, st = m[0], bd = 1;
        //vector<int> poss(na); iota(poss.begin(), poss.end(), 0); shuffle(poss.begin(), poss.end(), rnd);
        for (int ii = 0; ii < na; ++ii) {
            //const int i = poss[ii];
            const int i = ii;
            for (int j = i + 1; j + o <= na; ++j) {
                int d = m[j] - m[i], cnt = 2;
                int64_t nd = m[i] + 1ll * d * o;
                if (nd > m[na - 1]) break;
                int pos = mp[nd];
                if (pos - i + 1 <= o) continue;
                for (int c = m[j] + d; ; c += d) {
                    int u = mp[c];
                    if (u == -1 || u + (o - cnt) >= na) break;
                    cnt++;
                }
                if (cnt > o) o = cnt, st = m[i], bd = d;
            }
        }
        vector<int> ans(o);
        for (int c = st, i = 0; i < o; ++i, c += bd) {
            ans[i] = ps[mp[c]];
        }
        return ans;
    }
};
