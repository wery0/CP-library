template<typename T>
class segtree {

    struct Node {

        Node *l = 0, *r = 0;
        T sm = 0;

        Node() = default;

        Node(Node* n) {
            l = n->l, r = n->r;
            sm = n->sm;
        }
    };

    inline T gsm(Node* n) {return n ? n->sm : 0;}

    size_t n;
    vector<Node*> store;
    Node* root;

    void upd(Node* n) {
        n->sm = gsm(n->l) + gsm(n->r);
    }

    Node* point_add(size_t pos, size_t l, size_t r, Node* n, T add_val) {
        Node* nw = new Node(n);
        if (l == r) {
            nw->sm += add_val;
            return nw;
        }
        size_t md = (l + r) >> 1;
        if (pos <= md) nw->l = point_add(pos, l, md, n->l, add_val);
        else nw->r = point_add(pos, md + 1, r, n->r, add_val);
        upd(nw);
        return nw;
    }

    T seg_sum(size_t ql, size_t qr, size_t l, size_t r, Node* n) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) {
            return n->sm;
        }
        size_t md = (l + r) >> 1;
        const T lf = seg_sum(ql, qr, l, md, n->l);
        const T rg = seg_sum(ql, qr, md + 1, r, n->r);
        return lf + rg;
    }

public:
    segtree() = default;

    template<typename U>
    segtree(vector<U> m) {
        auto build = [&](auto&& build, Node*& n, int l, int r) -> void{
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
        n = m.size();
        build(build, root, 0, n - 1);
        store.push_back(root);
        umap<T, int> mp;
        for (size_t i = 0; i < n; ++i) {
            int rem = mp.count(m[i]) ? mp[m[i]] : -1;
            Node* nw = rem == -1 ? store.back() : point_add(rem, 0, n - 1, store.back(), -1);
            store.push_back(point_add(i, 0, n - 1, nw, 1));
            mp[m[i]] = i;
        }
    }

    T seg_sum(size_t version, size_t l, size_t r) {return seg_sum(l, r, 0, n - 1, store[version]);}

    void point_add(size_t version, size_t pos, T add_val, bool push_to_store) {
        Node* new_root = point_add(pos, 0, n - 1, store[version], add_val);
        if (push_to_store) store.push_back(new_root);
    }

    T seg_distinct(size_t l, size_t r) {
        return seg_sum(r + 1, l, r);
    }
};