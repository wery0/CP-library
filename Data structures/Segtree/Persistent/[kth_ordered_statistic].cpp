template<typename T>
struct segtree {

    struct Node {

        Node *l = 0, *r = 0;
        T sm = 0;

        Node() = default;

        Node(Node *n) {
            l = n->l, r = n->r;
            sm = n->sm;
        }
    };

    inline T gsm(Node *n) {return n ? n->sm : 0;}

    uint a;
    vec<Node*> store;
    Node* root;
    vec<T> was;

    segtree() = default;

    template<typename TT>
    segtree(vec<TT> m) {
        auto build = [&](auto && build, Node *&n, int l, int r) -> void{
            if (l > r) return;
            n = new Node();
            if (l == r) {
                n->sm = 0;
                return;
            }
            int md = (l + r) / 2;
            build(build, n->l, l, md);
            build(build, n->r, md + 1, r);
            upd(n);
        };
        a = m.size();
        vec<pii> wwas(a);
        for (int q = 0; q < a; ++q) wwas[q].S = m[q];
        {
            vec<T> n = m;
            unify(n);
            for (T& c : m) c = lower_bound(all(n), c) - n.begin();
        }
        for (int q = 0; q < a; ++q) wwas[q].F = m[q];
        unify(wwas);
        was.reserve(wwas.size());
        for (auto [nw, old] : wwas) was.pb(old);
        build(build, root, 0, a - 1);
        store.pb(root);
        for (int q = 0; q < a; q++) {
            store.pb(point_add(m[q], 0, a - 1, store[q], 1));
        }
    }

    void upd(Node *n) {
        n->sm = gsm(n->l) + gsm(n->r);
    }

    Node* point_add(uint pos, uint l, uint r, Node *n, T add_val) {
        Node *nw = new Node(n);
        if (l == r) {
            nw->sm += add_val;
            return nw;
        }
        uint md = (l + r) >> 1;
        if (pos <= md) nw->l = point_add(pos, l, md, n->l, add_val);
        else nw->r = point_add(pos, md + 1, r, n->r, add_val);
        upd(nw);
        return nw;
    }

    T seg_kth_order_statistics(int ql, int qr, int k) {
        Node *lf = store[ql], *rg = store[qr + 1];
        int l = 0, r = a - 1;
        for (; l < r;) {
            int md = (l + r) / 2;
            int l_val = gsm(rg->l) - gsm(lf->l);
            if (k < l_val) lf = lf->l, rg = rg->l, r = md;
            else k -= l_val, lf = lf->r, rg = rg->r, l = md + 1;
        }
        return was[l];
    }
};