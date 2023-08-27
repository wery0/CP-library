template<typename K>
class splay_tree {
    struct Node {
        Node* m[2] = {0}, *p = 0;
        K key;
        K mnk;
        size_t sz;
        bool rev = 0;

        Node() = default;

        Node(K k): sz(1) {
            key = k;
            mnk = k;
        }
    };
    Node* root = 0;

    size_t gsz(Node* n) const {return n ? n->sz : 0;}
    K gmnk(Node* n) const {return n ? n->mnk : std::numeric_limits<K>::max();}

    void push(Node* n) {
        if (!n) return;
        if (n->rev) {
            if (n->m[0]) n->m[0]->rev ^= 1;
            if (n->m[1]) n->m[1]->rev ^= 1;
            swap(n->m[0], n->m[1]);
            n->rev = 0;
        }
    }

    void super_push(Node* n) {
        static vector<Node*> store;
        while (n) {
            store.push_back(n);
            n = n->p;
        }
        while (store.size()) {
            push(store.back());
            store.pop_back();
        }
    }

    void upd(Node* n) {
        if (!n) return;
        n->mnk = min({gmnk(n->m[0]), n->key, gmnk(n->m[1])});

        n->sz = gsz(n->m[0]) + 1 + gsz(n->m[1]);
    }

    int dir(Node* n) const {return n->p->m[1] == n;}

    void disconnect(Node* n) {
        if (!n || !n->p) return;
        Node* p = n->p;
        p->m[dir(n)] = 0;
        n->p = 0;
        upd(p);
    }

    void connect(Node* p, Node* n, int d) {
        p->m[d] = n;
        if (n) n->p = p;
        upd(p);
    }

    void rotate(Node* n) {
        if (!n->p) return;
        Node* p = n->p, *g = n->p->p;
        int d = dir(n);
        if (g) {
            connect(g, n, dir(p));
        } else {
            n->p = 0;
        }
        connect(p, n->m[d ^ 1], d);
        connect(n, p, d ^ 1);
    }

    Node* splay(Node* n) {
        if (!n) return 0;
        super_push(n);
        while (n->p) {
            if (n->p->p) rotate(dir(n) == dir(n->p) ? n->p : n);
            rotate(n);
        }
        return n;
    }

    Node* leftmost(Node* n) {
        for (push(n); n->m[0]; push(n)) n = n->m[0];
        return n;
    }

    Node* rightmost(Node* n) {
        for (push(n); n->m[1]; push(n)) n = n->m[1];
        return n;
    }

    Node* merge(Node* l, Node* r) {
        if (!l || !r) return l ? l : r;
        l = splay(rightmost(l));
        connect(l, r, 1);
        return l;
    }

    Node* access(Node* n, size_t pos) {
        assert(0 <= pos && pos < gsz(n));
        while (n) {
            push(n);
            const size_t szl = gsz(n->m[0]);
            if (pos == szl) {
                root = splay(n);
                return root;
            }
            if (pos < szl) n = n->m[0];
            else pos -= szl + 1, n = n->m[1];
        }
        assert(0);
        return 0;
    }

    array<Node*, 2> split_size(Node* n, size_t k) {
        if (!k) return {0, n};
        assert(k <= gsz(n));
        n = access(n, k - 1);
        Node* r = n->m[1];
        disconnect(r);
        return {n, r};
    }

    template<typename I>
    Node* build(I f_key, I l_key) {
        if (f_key >= l_key) return 0;
        I md = f_key + (l_key - f_key) / 2;
        Node* n = new Node(*md);
        n->m[0] = build(f_key, md);
        n->m[1] = build(md + 1, l_key);
        if (n->m[0]) n->m[0]->p = n;
        if (n->m[1]) n->m[1]->p = n;
        upd(n);
        return n;
    }

public:
    splay_tree() = default;
    template<typename I> splay_tree(I f_key, I l_key) {root = build(f_key, l_key);}

    K seg_min(size_t l, size_t r) {
        auto [lf, tmp] = split_size(root, l);
        auto [mid, rg] = split_size(tmp, r - l + 1);
        K res = gmnk(mid);
        root = merge(merge(lf, mid), rg);
        return res;
    }

    void seg_rev(size_t l, size_t r) {
        auto [lf, tmp] = split_size(root, l);
        auto [mid, rg] = split_size(tmp, r - l + 1);
        mid->rev ^= 1;
        root = merge(merge(lf, mid), rg);
    }
};
