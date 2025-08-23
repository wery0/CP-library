template<typename T, typename C, bool do_compress, bool use_sum>
class wavelet_tree {
    static_assert(is_integral_v<T>);
    size_t n, lg;
    vector<T> keys;
    T mne, mxe;
    using bitvec = bit_vector<uint32_t, uint32_t>;
    vector<bitvec> bvs;
    vector<vector<C>> sums;

    C get_sum(size_t layer, size_t l, size_t r) {
        return sums[layer][r] - (l ? sums[layer][l - 1] : 0);
    }

    pair<int, C> seg_count_and_sum_leq_(int ql, int qr, T x) {
        assert(ql <= qr && qr < n);
        if (do_compress) {
            auto it = upper_bound(keys.begin(), keys.end(), x);
            if (it == keys.begin()) return {0, 0};
            x = it - keys.begin() - 1;
        }
        int l = 0, r = n - 1, cnt = 0;
        T vl = mne, vr = mxe;
        C sum = 0;
        for (int layer = 0; ql <= qr; ++layer) {
            if (vl == vr) {
                if (vl <= x) {
                    cnt += qr - ql + 1;
                    sum += get_sum(layer, ql, qr);
                }
                break;
            }
            T vm = vl + (vr - vl) / 2;
            int c0 = bvs[layer].seg0(l, r);
            int cq0 = bvs[layer].seg0(ql, qr);
            int cq1 = qr - ql + 1 - cq0;
            if (x <= vm) {
                ql = l + (ql ? bvs[layer].seg0(l, ql - 1) : 0);
                qr = ql + cq0 - 1;
                r = l + c0 - 1;
                vr = vm;
            } else {
                cnt += cq0;
                if constexpr (use_sum) sum += get_sum(layer, ql, qr);
                ql = l + c0 + (ql ? bvs[layer].seg1(l, ql - 1) : 0);
                qr = ql + cq1 - 1;
                l = l + c0;
                vl = vm + 1;
            }
        }
        return {cnt, sum};
    }

    pair<int, C> seg_count_and_sum_seg_(int ql, int qr, T x, T y) {
        assert(ql <= qr && qr < n);
        if (x > y) return {0, 0};
        if (do_compress) {
            auto ix = lower_bound(keys.begin(), keys.end(), x);
            if (ix == keys.begin()) return seg_count_and_sum_leq_(ql, qr, y);
            x = ix - keys.begin();
            auto iy = upper_bound(keys.begin(), keys.end(), y);
            y = iy - keys.begin() - 1;
        }
        int cnt = 0;
        C sum = 0;
        auto dfs = [&](auto&& dfs, int layer, int l, int r, int ql, int qr, T vl, T vr) {
            if (vr < x || y < vl || ql > qr || l > r) return;
            if (x <= vl && vr <= y) {
                cnt += qr - ql + 1;
                sum += get_sum(layer, ql, qr);
                return;
            }
            T vm = vl + (vr - vl) / 2;
            int c0 = bvs[layer].seg0(l, r);
            int cq0 = bvs[layer].seg0(ql, qr);
            int cq1 = qr - ql + 1 - cq0;
            int lfql = l + (ql ? bvs[layer].seg0(l, ql - 1) : 0), lfqr = lfql + cq0 - 1;
            dfs(dfs, layer + 1, l, l + c0 - 1, lfql, lfqr, vl, vm);
            int rgql = l + c0 + (ql ? bvs[layer].seg1(l, ql - 1) : 0), rgqr = rgql + cq1 - 1;
            dfs(dfs, layer + 1, l + c0, r, rgql, rgqr, vm + 1, vr);
        };
        dfs(dfs, 0, 0, n - 1, ql, qr, mne, mxe);
        return {cnt, sum};
    }

    pair<T, C> seg_kth_ordered_statistics_(int ql, int qr, int k) {
        assert(ql <= qr && qr < n);
        assert(k <= qr - ql);
        int l = 0, r = n - 1;
        T vl = mne, vr = mxe;
        C sum = 0;
        for (int layer = 0; vl < vr; ++layer) {
            T vm = vl + (vr - vl) / 2;
            int c0 = bvs[layer].seg0(l, r);
            int cq0 = bvs[layer].seg0(ql, qr);
            int cq1 = qr - ql + 1 - cq0;
            if (k < cq0) {
                ql = l + (ql ? bvs[layer].seg0(l, ql - 1) : 0);
                qr = ql + cq0 - 1;
                r = l + c0 - 1;
                vr = vm;
            } else {
                k -= cq0;
                if constexpr (use_sum) sum += get_sum(layer, ql, qr);
                ql = l + c0 + (ql ? bvs[layer].seg1(l, ql - 1) : 0);
                qr = ql + cq1 - 1;
                l = l + c0;
                vl = vm + 1;
            }
        }
        return {do_compress ? keys[vl] : vl, sum};
    }

public:
    wavelet_tree() = default;
    //use_sum == false, do_compress == false -> <O(nlog(C)), O(nlog(C)) bits>
    //use_sum == false, do_compress == true  -> <O(nlog(n)), O(nlog(n)) bits>
    //use_sum == true, do_compress == false  -> <O(nlog(C)), O(nlog(C)log(C)) bits>
    //use_sum == true, do_compress == true   -> <O(nlog(n)), O(nlog(n)log(C)) bits>
    template<typename I>
    wavelet_tree(I first, I last, vector<T> sum_data = {}): n(std::distance(first, last)) {
        if (use_sum) assert(sum_data.size() == n);
        if (!n) return;
        vector<T> m(first, last);
        vector<int> idx(n); iota(idx.begin(), idx.end(), 0);
        if (do_compress) {
            sort(idx.begin(), idx.end(), [&m](int l, int r) {return m[l] < m[r];});
            keys.reserve(n);
            for (int i : idx) {
                if (keys.empty() || keys.back() != m[i]) keys.emplace_back(m[i]);
                m[i] = keys.size() - 1;
            }
            keys.shrink_to_fit();
        }
        const auto [mni, mxi] = minmax_element(m.begin(), m.end());
        mne = *mni, mxe = *mxi;
        size_t D = mxe - mne + 1;
        lg = __lg(D) + (D & (D - 1) ? 2 : 1);
        bvs.resize(lg, bitvec(n));
        if constexpr (use_sum) sums.resize(lg, vector<C>(n));
        if (do_compress) iota(idx.begin(), idx.end(), 0);
        function<void(int, int, int, T, T)> build = [&](int layer, int l, int r, T vl, T vr) {
            if (l > r) return;
            T vm = vl + (vr - vl) / 2;
            for (int i = l; i <= r; ++i) {
                if (m[idx[i]] > vm) {
                    bvs[layer].set(i);
                } else if constexpr (use_sum) {
                    sums[layer][i] = sum_data[idx[i]];
                }
            }
            if (vl == vr) return;
            int p = stable_partition(idx.begin() + l, idx.begin() + r + 1, [&](int x) {return m[x] <= vm;}) - idx.begin();
            build(layer + 1, l, p - 1, vl, vm);
            build(layer + 1, p, r, vm + 1, vr);
        };
        build(0, 0, n - 1, mne, mxe);
        for (auto& bv : bvs) bv.prepare();
        for (auto& row : sums) partial_sum(row.begin(), row.end(), row.begin());
    }

    //O(log(# distinct)) if do_compress == true, O(log(mxe - mne)) otherwise
    int seg_count_leq(int l, int r, T x) {return seg_count_and_sum_leq_(l, r, x).first;}
    C seg_sum_leq(int l, int r, T x) {return seg_count_and_sum_leq_(l, r, x).second;}
    pair<int, C> seg_count_and_sum_leq(int l, int r, T x) {return seg_count_and_sum_leq_(l, r, x);}
    int seg_count_seg(int l, int r, T x, T y) {return seg_count_and_sum_seg_(l, r, x, y).first;}
    C seg_sum_seg(int l, int r, T x, T y) {return seg_count_and_sum_seg_(l, r, x, y).second;}
    pair<int, C> seg_count_and_sum_seg(int l, int r, T x, T y) {return seg_count_and_sum_seg_(l, r, x, y);}
    T seg_kth_ordered_statistics(int l, int r, int k) {return seg_kth_ordered_statistics_(l, r, k).first;}
    C seg_kth_ordered_statistics_sum(int l, int r, int k) {return seg_kth_ordered_statistics_(l, r, k).second;}
    pair<T, C> seg_kth_ordered_statistics_and_sum(int l, int r, int k) {return seg_kth_ordered_statistics_(l, r, k);}
};
