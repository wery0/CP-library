template<typename T>
struct longest_increasing_arithmetic_progression {

    template<typename K, typename V, const int LG>
    struct hash_map_open_addressing {

        const int N = 1 << LG;
        vec<pair<K, V>> store = vec<pair<K, V>>(N);
        vec<bool> is_occupied = vec<bool>(N);

        constexpr inline void next_pos(int &pos) const {
            pos = pos < N - 1 ? pos + 1 : 0;
        }

        static constexpr ull kek = 11995408973635179863ull;
        constexpr inline int hsh(const K& key) const {
            if constexpr(std::is_integral<K>::value) {
                //return key & (N - 1);
                return (key * kek) >> (64 - LG);
            } else {
                return hash<K> {}(key) % N;
            }
        }

        inline void clear() {
            fill(all(is_occupied), false);
        }

        inline V operator[](const K& key) const {
            int pos = hsh(key);
            for (; is_occupied[pos]; next_pos(pos)) {
                if (store[pos].F == key) return store[pos].S;
            }
            return -1;
        }

        inline bool contains(const K& key) const {
            int pos = hsh(key);
            for (; is_occupied[pos]; next_pos(pos)) {
                if (store[pos].F == key) return true;
            }
            return false;
        }

        inline void add(const K& key, const V& val) {
            int pos = hsh(key);
            for (; is_occupied[pos]; next_pos(pos)) {
                if (store[pos].F == key) {
                    store[pos].S = val;
                    return;
                }
            }
            store[pos] = {key, val};
            is_occupied[pos] = 1;
        }
    };

    hash_map_open_addressing < T, int, 18 > mp; //change, if need
    vec<int> solve(vec<T> m) {
        int a = m.size(), na = 0;
        vec<pair<T, int>> stm(a);
        vec<int> ps(a);
        for (int q = 0; q < a; ++q) stm[q] = {m[q], q};
        sort(all(stm));
        for (int q = 0; q < a; q++) {
            if (q && stm[q].F == stm[q - 1].F) continue;
            m[na] = stm[q].F;
            mp.add(m[na], na);
            ps[na++] = stm[q].S;
        }
        int o = 1, st = m[0], bd = 1;
        //vec<int> poss(na); iota(all(poss), 0);shuffle(all(poss), rnd);
        for (int qq = 0; qq < na; qq++) {
            //const int q = poss[qq];
            const int q = qq;
            for (int w = q + 1; w + o <= na; w++) {
                int d = m[w] - m[q], cnt = 2;
                ll nd = m[q] + 1LL * d * o;
                if (nd > m[na - 1]) break;
                int pos = mp[nd];
                if (pos - q + 1 <= o) continue;
                for (int c = m[w] + d; ; c += d) {
                    int u = mp[c];
                    if (u == -1 || u + (o - cnt) >= na) break;
                    cnt++;
                }
                if (chmax(o, cnt)) st = m[q], bd = d;
            }
        }
        vec<int> ans(o);
        for (int c = st, i = 0; i < o; ++i, c += bd) {
            ans[i] = ps[mp[c]];
        }
        return ans;
    }
};
//returns positions of progression element's
//https://acm.timus.ru/problem.aspx?space=1&num=1395