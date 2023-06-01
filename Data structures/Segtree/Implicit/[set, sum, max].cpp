template<typename T, typename C, C L, C R>      //Works with coordinates [L; R]
class implicit_segtree {
    static constexpr T NO = -1;     //Change, if set value can be -1
    static constexpr T INF = numeric_limits<T>::max();
    struct Node {
        Node* l = 0;
        Node* r = 0;
        T ps_set = NO;
        T sm = 0;
        T mx = -INF;

        Node() = default;
    };

    Node* root = new Node();

    T gsm(Node* n) const {return n ? n->sm : 0;}
    T gmx(Node* n) const {return n ? n->mx : -INF;}

    void setval(Node* n, T val, T sz) {
        n->ps_set = val;
        n->sm = sz * val;
        n->mx = val;
    }

    void push(Node* n, T sz) {
        if (!n->l) n->l = new Node();
        if (!n->r) n->r = new Node();
        setval(n->l, n->ps_set, (sz + 1) >> 1);
        setval(n->r, n->ps_set, sz >> 1);
        n->ps_set = NO;
    }

    void upd(Node* n) {
        n->sm = gsm(n->l) + gsm(n->r);
        n->mx = max(gmx(n->l), gmx(n->r));
    }

    void seg_set(T ql, T qr, T val, C l, C r, Node*& n) {
        if (qr < l || r < ql) return;
        if (!n) n = new Node();
        if (ql <= l && r <= qr) {
            setval(n, val, r - l + 1);
            return;
        }
        if (n->ps_set != NO) push(n, r - l + 1);
        const C md = l + (r - l) / 2;
        seg_set(ql, qr, val, l, md, n->l);
        seg_set(ql, qr, val, md + 1, r, n->r);
        upd(n);
    }

    T seg_sum(T ql, T qr, C l, C r, Node* n) const {
        if (!n || qr < l || r < ql) return 0;
        if (n->ps_set != NO) return (min(qr, r) - max(ql, l) + 1) * n->ps_set;
        if (ql <= l && r <= qr) return n->sm;
        const C md = l + (r - l) / 2;
        return seg_sum(ql, qr, l, md, n->l) +
               seg_sum(ql, qr, md + 1, r, n->r);
    }

    T seg_max(T ql, T qr, C l, C r, Node* n) const {
        if (!n || qr < l || r < ql) return -INF;
        if (n->ps_set != NO) return n->ps_set;
        if (ql <= l && r <= qr) return n->mx;
        const C md = l + (r - l) / 2;
        return max(seg_max(ql, qr, l, md, n->l),
                   seg_max(ql, qr, md + 1, r, n->r));
    }

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->l);
        destroy(n->r);
        delete n;
    }

public:
    implicit_segtree() = default;
    ~implicit_segtree() {destroy(root);}

    T seg_sum(C ql, C qr) const {return seg_sum(ql, qr, L, R, root);}
    T seg_max(C ql, C qr) const {return seg_max(ql, qr, L, R, root);}
    void seg_set(C ql, C qr, T val) {seg_set(ql, qr, val, L, R, root);}
};