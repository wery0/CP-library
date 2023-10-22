template<typename T, typename C, C L, C R>  //Works with coordinates [L; R]
class implicit_segtree {
    
    static constexpr T NO_PUSH_SET = numeric_limits<T>::max() - sqrt(numeric_limits<T>::max());  //Change, if set value can be -1
    
    struct Node {
        Node* l = 0;
        Node* r = 0;
        T sm = 0;
        T ps_set = NO_PUSH_SET;

        Node() = default;
    };

    Node* root = new Node();

    T gsm(Node* n) const {return n ? n->sm : 0;}

    void setval(Node* n, T val, C l, C r) {
        n->ps_set = val;
        n->sm = (T)(l + r) * (r - l + 1) * val;
    }

    void push(Node* n, C l, C r) {
        if (!n->l) n->l = new Node();
        if (!n->r) n->r = new Node();
        const C md = l + (r - l) / 2;
        setval(n->l, n->ps_set, l, md);
        setval(n->r, n->ps_set, md + 1, r);
        n->ps_set = NO_PUSH_SET;
    }

    void upd(Node* n) {
        n->sm = gsm(n->l) + gsm(n->r);
    }

    T seg_sum(C ql, C qr, C l, C r, Node* n) const {
        if (!n || qr < l || r < ql) return 0;
        if (n->ps_set != NO_PUSH_SET) {
            T x = max(ql, l), y = min(qr, r);
            return (x + y) * (y - x + 1) * n->ps_set;
        }
        if (ql <= l && r <= qr) return n->sm;
        const C md = l + (r - l) / 2;
        return seg_sum(ql, qr, l, md, n->l) +
               seg_sum(ql, qr, md + 1, r, n->r);
    }

    void seg_set(C ql, C qr, C l, C r, Node*& n, T val) {
        if (qr < l || r < ql) return;
        if (!n) n = new Node();
        if (ql <= l && r <= qr) {
            setval(n, val, l, r);
            return;
        }
        if (n->ps_set != NO_PUSH_SET) push(n, l, r);
        const C md = l + (r - l) / 2;
        seg_set(ql, qr, l, md, n->l, val);
        seg_set(ql, qr, md + 1, r, n->r, val);
        upd(n);
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

    //Calculates sum{i = l}{r}{i * a[i]}
    T seg_sum(C l, C r) const {return seg_sum(l, r, L, R, root) / 2;}
    void seg_set(C l, C r, T val) {seg_set(l, r, L, R, root, val);}
};
