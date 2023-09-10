template<typename T>
class merge_sort_tree {

    static constexpr T inf = numeric_limits<T>::max();

    size_t n, U, M;
    vector<T> store;
    vector<int> st;

    int work_seg(size_t i, T val_l, T val_r) {
        auto it_l = lower_bound(store.begin() + st[i], store.begin() + st[i + 1], val_l);
        auto it_r = upper_bound(store.begin() + st[i], store.begin() + st[i + 1], val_r);
        return it_r - it_l;
    }

    int work_leq(size_t i, T val_r) {
        auto it_r = upper_bound(store.begin() + st[i], store.begin() + st[i + 1], val_r);
        return it_r - store.begin() - st[i];
    }

    T work_max_leq(size_t i, T x, T NO_ANSWER) {
        auto it_r = upper_bound(store.begin() + st[i], store.begin() + st[i + 1], x);
        return it_r == store.begin() + st[i] ? NO_ANSWER : *--it_r;
    }

public:
    template<typename Iterator>
    merge_sort_tree(Iterator first, Iterator last): n(last - first), U(n & (n - 1) ? 2 << __lg(n) : n), store(U * 2) {
        if (n == 0) return;
        M = (n ? __lg(n) + 1 : 0) * n + (n & (n - 1) ? n : 0);
        store.resize(M);
        st.resize(U * 2 + 1);
        auto go = [&](auto&& go, size_t l, size_t r, size_t v, size_t dep = 0) -> void {
            st[v] = n * dep + min(l, n);
            if (l == r) return;
            size_t md = (l + r) >> 1;
            go(go, l, md, v << 1, dep + 1);
            go(go, md + 1, r, v << 1 | 1, dep + 1);
        };
        go(go, 0, U - 1, 1);
        st.back() = M;
        for (size_t i = 0; i < n; ++i, ++first) {
            store[st[U + i]] = *first;
        }
        for (size_t i = U; --i;) {
            merge(store.begin() + st[i << 1], store.begin() + st[i << 1 | 1], store.begin() + st[i << 1 | 1], store.begin() + st[(i << 1) + 2], store.begin() + st[i]);
        }
    }

    template<typename T_arr>
    merge_sort_tree(T_arr& m) {
        (*this) = merge_sort_tree(m.begin(), m.end());
    }

    //Returns # values in range [val_l, val_r] on segment [l, r]
    //O(log(n)^2)
    int seg_count_seg(size_t l, size_t r, T val_l, T val_r) {
        l += U, r += U;
        int ans = 0;
        while (l <= r) {
            if (l & 1) ans += work_seg(l, val_l, val_r);
            if (~r & 1) ans += work_seg(r, val_l, val_r);
            l = (l + 1) >> 1;
            r = (r - 1) >> 1;
        }
        return ans;
    }

    //Returns # values <= val_r on segment [l, r]
    //O(log(n)^2)
    int seg_count_leq(size_t l, size_t r, T val_r) {
        l += U, r += U;
        int ans = 0;
        while (l <= r) {
            if (l & 1) ans += work_leq(l, val_r);
            if (~r & 1) ans += work_leq(r, val_r);
            l = (l + 1) >> 1;
            r = (r - 1) >> 1;
        }
        return ans;
    }

    //Returns maximum number on segment [l, r], which <= x
    //If all numbers on segment [l, r] > x, then returns:
    //  NO_ANSWER, if it`s set
    //  x + 1      otherwise
    //O(log(n)^2)
    T seg_max_leq(size_t l, size_t r, T x, T NO_ANSWER = -inf) {
        l += U, r += U;
        T ans = -inf;
        while (l <= r) {
            if (l & 1) ans = max(ans, work_max_leq(l, x, -inf));
            if (~r & 1) ans = max(ans, work_max_leq(r, x, -inf));
            l = (l + 1) >> 1;
            r = (r - 1) >> 1;
        }
        return ans != -inf ? ans : NO_ANSWER == -inf ? x + 1 : NO_ANSWER;
    }
};
