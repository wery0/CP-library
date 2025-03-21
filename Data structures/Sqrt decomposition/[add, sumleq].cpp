template<typename K, typename V>
class seg_add_sumleq {
    size_t N, B, nB;
    vector<int> st;
    vector<int> si;
    vector<K> keys;
    vector<K> psk;
    K gpsk = 0;
    vector<V> values;
    vector<V> smsv;
    vector<K> bk;

    void calc_smsv(size_t b) {
        for (int i = st[b]; i < st[b + 1]; ++i) smsv[i] = values[si[i]];
        partial_sum(smsv.begin() + st[b], smsv.begin() + st[b + 1], smsv.begin() + st[b]);
    }

    void resort(size_t b) {
        sort(si.begin() + st[b], si.begin() + st[b + 1], [&](int i, int j) {return keys[i] < keys[j];});
        calc_smsv(b);
    }

    size_t block_length(size_t i) const {return st[i + 1] - st[i];}
    size_t get_block(size_t i) const {return i / B;}

public:
    seg_add_sumleq() = default;
    seg_add_sumleq(vector<K>& keys, vector<V>& values): N(keys.size()), B(sqrt(N * __lg(N))), nB((N + B - 1) / B), st(nB + 1, N), si(N), psk(nB), keys(keys), values(values), smsv(N), bk(B * 2) {
        assert(keys.size() == values.size());
        for (int i = 0; i < nB; ++i) st[i] = i * B;
        iota(si.begin(), si.end(), 0);
        for (int b = 0; b < nB; ++b) resort(b);
    }

    void seg_add_to_keys(size_t l, size_t r, K x) {
        auto smart = [&](size_t b) {
            psk[b] += x;
        };
        auto stupid = [&](size_t l, size_t r) {
            size_t b = get_block(l);
            for (size_t i = l; i <= r; ++i) keys[i] += x;
            int s1 = 0, s2 = r - l + 1;
            for (int i = st[b]; i < st[b + 1]; ++i) {
                bk[l <= si[i] && si[i] <= r ? s1++ : s2++] = si[i];
            }
            merge(bk.begin(), bk.begin() + s1, bk.begin() + s1, bk.begin() + s2, si.begin() + st[b], [&](int i, int j) {return keys[i] < keys[j];});
            calc_smsv(b);
        };
        size_t bl = get_block(l);
        size_t br = get_block(r);
        if (bl == br) stupid(l, r);
        else {
            if (l != st[bl]) stupid(l, st[bl + 1] - 1), ++bl;
            if (r + 1 != st[br + 1]) stupid(st[br], r), --br;
            for (; bl <= br; ++bl) smart(bl);
        }
    }

    void global_add_to_keys(K x) {
        gpsk += x;
    }

    V seg_sum_values_with_key_leq_x(size_t l, size_t r, K x) {
        V res = 0;
        auto smart = [&](size_t b) {
            int d;
            if (keys[si[st[b]]] + psk[b] + gpsk > x) d = 0;
            else if (keys[si[st[b + 1] - 1]] + psk[b] + gpsk <= x) d = block_length(b);
            else d = upper_bound(si.begin() + st[b], si.begin() + st[b + 1], x - psk[b] - gpsk, [&](K k, int it) {return k < keys[it];}) - si.begin() - st[b];
            if (d) res += smsv[st[b] + d - 1];
        };
        auto stupid = [&](size_t l, size_t r) {
            size_t b = get_block(l);
            const K nx = x - psk[b] - gpsk;
            for (size_t i = l; i <= r; ++i) res += keys[i] <= nx ? values[i] : 0;
        };
        size_t bl = get_block(l);
        size_t br = get_block(r);
        if (bl == br) stupid(l, r);
        else {
            if (l != st[bl]) stupid(l, st[bl + 1] - 1), ++bl;
            if (r + 1 != st[br + 1]) stupid(st[br], r), --br;
            for (; bl <= br; ++bl) smart(bl);
        }
        return res;
    }
};
