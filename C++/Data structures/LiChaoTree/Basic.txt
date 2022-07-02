template<typename T>
struct lichao {

    static constexpr T inf = numeric_limits<T>::max();

    struct Node {
        Node *l = 0, *r = 0;
        T k = 0, b = inf;

        Node() {}
    };

    T L, R;
    Node *root = new Node();

    lichao(T _L, T _R) {
        L = _L;
        R = _R;
    }

    void add_seg(T ql, T qr, T l, T r, T k, T b, Node*& n) {
        if (qr < l || r < ql) return;
        if (!n) n = new Node();
        T md = l + (r - l) / 2;
        if (!(ql <= l && r <= qr)) {
            add_seg(ql, qr, l, md, k, b, n->l);
            add_seg(ql, qr, md + 1, r, k, b, n->r);
            return;
        }
        add_line(l, r, k, b, n);
    }
    void add_seg(T ql, T qr, T k, T b) {add_seg(ql, qr, L, R, k, b, root);}

    void add_line(T l, T r, T k, T b, Node*& n) {
        if (!n) n = new Node();
        T vl_cur = n->k * l + n->b;
        T vr_cur = n->k * r + n->b;
        T vl_new = k * l + b;
        T vr_new = k * r + b;
        if ((vl_cur <= vl_new) == (vr_cur <= vr_new)) {
            if (vl_new < vl_cur) n->k = k, n->b = b;
            return;
        }
        T md = l + (r - l) / 2;
        T vmd_cur = n->k * md + n->b;
        T vmd_new = k * md + b;
        if (vmd_new < vmd_cur) {
            swap(n->k, k), swap(n->b, b);
            swap(vl_cur, vl_new);
        }
        if (vl_new < vl_cur) add_line(l, md, k, b, n->l);
        else add_line(md + 1, r, k, b, n->r);
    }
    void add_line(T k, T b) {add_line(L, R, k, b, root);}

    T get_min(T x) {
        Node *n = root;
        T o = inf, l = L, r = R;
        while (n) {
            chmin(o, n->k * x + n->b);
            T md = l + (r - l) / 2;
            (x <= md ? r : l) = md + (x > md);
            n = x <= md ? n->l : n->r;
        }
        return o;
    }
};
//Add line - O(log(C))
//Add seg - O(log(C)^2)