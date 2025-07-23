template<typename T>
class segtree {

    struct Node {

        Node* l = 0, *r = 0;
        T sm = 0;

        Node() = default;

        Node(Node* n) {
            l = n->l, r = n->r;
            sm = n->sm;
        }
    };

    T gsm(Node* n) const {return n ? n->sm : 0;}

    size_t n;
    vector<Node*> store;
    Node* root;
    vector<T> was;

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

public:
    segtree() = default;

    template<typename U>
    segtree(const vector<U>& m): n(m.size()) {
        auto build = [&](auto&& build, Node*& n, int l, int r) -> void {
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
        vector<pair<int, T>> wwas(n);
        for (size_t i = 0; i < n; ++i) wwas[i].second = m[i];
        vector<int> nm(n);
        {
            vector<T> cm = m;
            sort(cm.begin(), cm.end());
            cm.erase(unique(cm.begin(), cm.end()), cm.end());
            for (int i = 0; i < n; ++i) nm[i] = lower_bound(cm.begin(), cm.end(), m[i]) - cm.begin();
        }
        for (size_t i = 0; i < n; ++i) wwas[i].first = nm[i];
        sort(wwas.begin(), wwas.end());
        wwas.erase(unique(wwas.begin(), wwas.end()), wwas.end());
        was.reserve(wwas.size());
        for (auto [nw, old] : wwas) was.push_back(old);
        build(build, root, 0, n - 1);
        store.push_back(root);
        for (size_t i = 0; i < n; ++i) {
            store.push_back(point_add(nm[i], 0, n - 1, store[i], 1));
        }
    }

    T seg_kth_order_statistics(size_t l, size_t r, T k) const {
        Node* lf = store[l], *rg = store[r + 1];
        l = 0, r = n - 1;
        while (l < r) {
            size_t md = (l + r) / 2;
            T l_val = gsm(rg->l) - gsm(lf->l);
            if (k < l_val) lf = lf->l, rg = rg->l, r = md;
            else k -= l_val, lf = lf->r, rg = rg->r, l = md + 1;
        }
        return was[l];
    }
};
