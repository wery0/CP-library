template<typename X, typename Y>
class lichao {

    static constexpr Y inf = numeric_limits<Y>::max();

    struct func {
        Y k, b;

        func() = default;
        func(Y k, Y b): k(k), b(b) {}

        Y eval(X x) const {
            return k * x + b;
        }
    };

    struct Node {
        Node *l = 0, *r = 0;
        func f{0, inf};

        Node() = default;
    };

    X L, R;
    Node* root = new Node();

    void add_seg(X ql, X qr, X l, X r, func nf, Node*& n) {
        if (qr < l || r < ql) return;
        if (!n) n = new Node();
        X md = l + (r - l) / 2;
        if (l < ql || r > qr) {
            add_seg(ql, qr, l, md, nf, n->l);
            add_seg(ql, qr, md + 1, r, nf, n->r);
            return;
        }
        add_line(l, r, nf, n);
    }

    void add_line(X l, X r, func nf, Node*& n) {
        if (!n) n = new Node();
        Y vl_cur = n->f.eval(l);
        Y vr_cur = n->f.eval(r);
        Y vl_new = nf.eval(l);
        Y vr_new = nf.eval(r);
        if ((vl_cur <= vl_new) == (vr_cur <= vr_new)) {
            if (vl_new < vl_cur) n->f = nf;
            return;
        }
        X md = l + (r - l) / 2;
        Y vmd_cur = n->f.eval(md);
        Y vmd_new = nf.eval(md);
        if (vmd_new < vmd_cur) {
        	swap(n->f, nf);
            swap(vl_cur, vl_new);
        }
        if (vl_new < vl_cur) add_line(l, md, nf, n->l);
        else add_line(md + 1, r, nf, n->r);
    }

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->l);
        destroy(n->r);
        delete n;
    }

public:
    lichao() = default;
    lichao(X L, X R): L(L), R(R) {}
    ~lichao() {destroy(root);}

    //O(log(C))
    Y get_min(X x) {
        Node* n = root;
        X l = L, r = R;
        Y o = inf;
        while (n) {
            o = min(o, n->f.eval(x));
            X md = l + (r - l) / 2;
            (x <= md ? r : l) = md + (x > md);
            n = x <= md ? n->l : n->r;
        }
        return o;
    }

    //O(log(C))
    void add_line(Y k, Y b) {add_line(L, R, func(k, b), root);}

    //O(log(C) ^ 2)
    void add_seg(X l, X r, Y k, Y b) {add_seg(l, r, L, R, func(k, b), root);}
};
//Usage: lichao<X, Y> kek(L, R), where
//X - type of coordinates,
//Y - type of K and B in linear function y = Kx + B,
//L, R - bounds of X.
