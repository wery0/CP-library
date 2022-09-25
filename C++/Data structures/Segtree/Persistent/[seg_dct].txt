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
        build(build, root, 0, a - 1);
        store.pb(root);
        umap<T, int> mp;
        for (int q = 0; q < a; q++) {
            int rem = mp.count(m[q]) ? mp[m[q]] : -1;
            Node *n = rem == -1 ? store.back() : point_add(rem, 0, a - 1, store.back(), -1);
            store.pb(point_add(q, 0, a - 1, n, 1));
            mp[m[q]] = q;
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
    inline void point_add(uint version, uint pos, T add_val, bool push_to_store) {
        Node* new_root = point_add(pos, 0, a - 1, store[version], add_val);
        if (push_to_store) store.pb(new_root);
    }

    T seg_sum(uint ql, uint qr, uint l, uint r, Node *n) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) {
            return n->sm;
        }
        uint md = (l + r) >> 1;
        const T lf = seg_sum(ql, qr, l, md, n->l);
        const T rg = seg_sum(ql, qr, md + 1, r, n->r);
        return lf + rg;
    }
    T seg_sum(uint version, uint ql, uint qr) {return seg_sum(ql, qr, 0, a - 1, store[version]);}

    T seg_distinct(int ql, int qr) {
        return seg_sum(qr + 1, ql, qr);
    }
};