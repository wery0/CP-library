template<typename T>
class static_range_mode {
    const size_t n, B, nB;
    vector<int> m;
    vector<T> i_item;
    unordered_map<T, int> item_i;
    vector<int> pps;
    vector<vector<int>> ps;
    vector<vector<array<int, 2>>> bmc;

    static constexpr size_t calc_B(size_t n) {
        return 1 << (__lg(n + 1) / 2);
    }

public:
    static_range_mode() = default;

    //<O(nlog(n) + n ^ 2 / B), O(n + (n / B) ^ 2)>
    template<typename I>
    static_range_mode(I first, I last): n(std::distance(first, last)), B(calc_B(n)), nB((n + B - 1) / B), m(n), i_item(n) {
        if (!n) return;
        assert(B);
        copy(first, last, i_item.begin());
        sort(i_item.begin(), i_item.end());
        i_item.erase(unique(i_item.begin(), i_item.end()), i_item.end());
        const size_t D = i_item.size();
        item_i.reserve(D);
        for (size_t i = 0; const T& item : i_item) item_i[item] = i++;
        for (size_t i = 0; first != last; ++first) m[i++] = item_i[*first];
        ps.resize(D);
        pps.resize(n);
        for (int i = 0; i < n; ++i) {
            if (ps[m[i]].empty()) ps[m[i]] = {0};
            pps[i] = ps[m[i]].size();
            ps[m[i]].push_back(i + 1);
        }
        for (auto& p : ps) p.push_back(n + 1);
        bmc.resize(nB, vector<array<int, 2>>(nB));
        vector<int> cnt(D);
        for (size_t l = 0; l < nB; ++l) {
            fill(cnt.begin(), cnt.end(), 0);
            int mode = 0, cm = 0;
            for (size_t r = l; r < nB; ++r) {
                const size_t lf = r * B, rg = min(n, lf + B);
                for (size_t i = lf; i < rg; ++i) {
                    if (++cnt[m[i]] > cm || (cnt[m[i]] == cm && m[i] > mode)) {
                        mode = m[i];
                        cm = cnt[m[i]];
                    }
                }
                bmc[l][r] = {mode, cm};
            }
        }
    }

    //Returns {mode, # occurrences of mode}
    //If there are several modes, returns maximum of them
    //O(B)
    pair<T, size_t> seg_mode(size_t l, size_t r) const {
        assert(l <= r && r < n);
        static int qn = 0; ++qn;
        static vector<int> us;
        if (us.empty()) us.resize(n);
        const size_t bl = l / B, br = r / B;
        int mode = 0, cnt = 0;
        auto walkthrough = [&](size_t st, size_t fn) {
            const bool lr = st < fn;
            const int dir = lr ? 1 : -1;
            for (size_t i = st; i != fn; i += dir) {
                if (us[m[i]] == qn) continue;
                us[m[i]] = qn;
                const auto& p = ps[m[i]];
                int pl = pps[i];
                int nd = pl + (m[i] > mode ? cnt - 1 : cnt) * (lr ? 1 : -1);
                if ((lr && nd + 1 < p.size() && p[nd] <= r + 1) || (!lr && nd > 0 && p[nd] >= l + 1)) {
                    mode = m[i];
                    cnt = (lr ? nd - pl : pl - nd) + 1;
                    if (lr) {
                        for (size_t j = nd + 1; p[j] <= r + 1; ++j) ++cnt;
                    } else {
                        for (size_t j = nd - 1; p[j] >= l + 1; --j) ++cnt;
                    }
                }
            }
        };
        if (br - bl > 1) {
            mode = bmc[bl + 1][br - 1][0];
            cnt = bmc[bl + 1][br - 1][1];
        }
        if (bl == br) {
            walkthrough(l, r + 1);
        } else {
            walkthrough(l, bl * B + B);
            walkthrough(r, br * B - 1);
        }
        return {i_item[mode], cnt};
    }
};
