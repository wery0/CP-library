template<typename X, typename Y>
class lichao {

    static constexpr Y inf = numeric_limits<Y>::max();

    struct Node {
        Node *l = 0, *r = 0;
        Y k = 0, b = inf;

        Node() = default;
    };

    X L, R;
    Node *root = new Node();

    void add_seg(X ql, X qr, X l, X r, Y k, Y b, Node*& n) {
        if (qr < l || r < ql) return;
        if (!n) n = new Node();
        X md = l + (r - l) / 2;
        if (l < ql || r > qr) {
            add_seg(ql, qr, l, md, k, b, n->l);
            add_seg(ql, qr, md + 1, r, k, b, n->r);
            return;
        }
        add_line(l, r, k, b, n);
    }

    void add_line(X l, X r, Y k, Y b, Node*& n) {
        if (!n) n = new Node();
        Y vl_cur = n->k * l + n->b;
        Y vr_cur = n->k * r + n->b;
        Y vl_new = k * l + b;
        Y vr_new = k * r + b;
        if ((vl_cur <= vl_new) == (vr_cur <= vr_new)) {
            if (vl_new < vl_cur) n->k = k, n->b = b;
            return;
        }
        X md = l + (r - l) / 2;
        Y vmd_cur = n->k * md + n->b;
        Y vmd_new = k * md + b;
        if (vmd_new < vmd_cur) {
            swap(n->k, k), swap(n->b, b);
            swap(vl_cur, vl_new);
        }
        if (vl_new < vl_cur) add_line(l, md, k, b, n->l);
        else add_line(md + 1, r, k, b, n->r);
    }

public:
    lichao(X L, X R): L(L), R(R) {}

    Y get_min(X x) {
        Node *n = root;
        X l = L, r = R;
        Y o = inf;
        while (n) {
            o = min(o, n->k * x + n->b);
            X md = l + (r - l) / 2;
            (x <= md ? r : l) = md + (x > md);
            n = x <= md ? n->l : n->r;
        }
        return o;
    }

    void add_line(Y k, Y b) {add_line(L, R, k, b, root);}
    void add_seg(X ql, X qr, Y k, Y b) {add_seg(ql, qr, L, R, k, b, root);}
};
//add_line - O(log(C))
//add_seg - O(log(C) ^ 2)
//Usage: lichao<X, Y> kek(L, R), where
//X - type of coordinates,
//Y - type of K and B in linear function y = Kx + B,
//L, R - bounds of X.