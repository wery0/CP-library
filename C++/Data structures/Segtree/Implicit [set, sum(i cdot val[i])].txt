const ll mod = 998244353;
inline ll sum_arifm_progr(ll fir, ll last) {
    ll cnt = (last - fir + 1) % mod;
    return (fir + last) % mod * cnt % mod * 499122177 % mod;
}

template<ll G>
struct implicit_segtree {

    ll seg_sum(ll ql, ll qr) {
        return seg_sum(ql, qr, 0, G - 1, root);
    }

    void seg_set(ll ql, ll qr, ll val) {
        seg_set(ql, qr, val, 0, G - 1, root);
    }

private:
    static const int NO = -1;  //change, if set value can be -1
    struct Node {
        Node *l = 0;
        Node *r = 0;
        int ps_set = NO, sm = 0;

        Node() {}
    };

    Node *root = new Node();

    inline int gsm(Node *n) {return n ? n->sm : 0;}

    inline void setval(Node *n, ll val, ll l, ll r) {
        val %= mod;
        n->ps_set = val;
        n->sm = 1LL * sum_arifm_progr(l, r) * val % mod;
    }

    inline void push(Node *n, ll l, ll r) {
        if (!n->l) n->l = new Node();
        if (!n->r) n->r = new Node();
        ll md = (l + r) >> 1;
        setval(n->l, n->ps_set, l, md);
        setval(n->r, n->ps_set, md + 1, r);
        n->ps_set = NO;
    }

    inline void upd(Node *n) {
        n->sm = (gsm(n->l) + gsm(n->r)) % mod;
    }

    implicit_segtree() {}

    void seg_set(ll ql, ll qr, ll val, ll l, ll r, Node *&n) {
        if (qr < l || r < ql) return;
        if (!n) n = new Node();
        if (ql <= l && r <= qr) {
            setval(n, val, l, r);
            return;
        }
        if (n->ps_set != NO) push(n, l, r);
        const ll md = (l + r) >> 1;
        seg_set(ql, qr, val, l, md, n->l);
        seg_set(ql, qr, val, md + 1, r, n->r);
        upd(n);
    }

    ll seg_sum(ll ql, ll qr, ll l, ll r, Node *&n) {
        if (!n || qr < l || r < ql) return 0;
        if (n->ps_set != NO) return 1LL * sum_arifm_progr(max(ql, l), min(qr, r)) * n->ps_set % mod;
        if (ql <= l && r <= qr) {
            return n->sm;
        }
        const ll md = (l + r) >> 1;
        return seg_sum(ql, qr, l, md, n->l) +
               seg_sum(ql, qr, md + 1, r, n->r);
    }
};