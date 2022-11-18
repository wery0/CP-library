template<typename K, typename V>
struct treap {
public:
    static const V inf = std::numeric_limits<V>::max();
    static const V UNDEF = 0;
    struct Node {
        Node *l = 0;
        Node *r = 0;
        K x;
        K mnk;
        K mxk;
        K smk;
        V val;
        V mnv;
        V mxv;
        V smv;
        ll y;
        ll sz = 0;

        Node(K x, V val = UNDEF): x(x), val(val), y(rnd()), sz(1) {
            mnk = x;
            mxk = x;
            smk = x;
            mnv = val;
            mxv = val;
            smv = val;
        }
    };
    Node *root = 0;

private:
    K last_erased_key;
    V last_erased_val;

    K gmnk(Node *n) const {return n ? n->mnk : inf;}
    K gmxk(Node *n) const {return n ? n->mxk : -inf;}
    K gsmk(Node *n) const {return n ? n->smk : 0;}

    V gmnv(Node *n) const {return n ? n->mnv : inf;}
    V gmxv(Node *n) const {return n ? n->mxv : -inf;}
    V gsmv(Node *n) const {return n ? n->smv : 0;}

    ll gsz(Node *n) const {return n ? n->sz : 0;}

    inline void push(Node *n) {
        if (!n) return;
    }

    inline void upd(Node *n) {
        n->mnk = min({gmnk(n->l), n->x, gmnk(n->r)});
        n->mxk = max({gmxk(n->l), n->x, gmxk(n->r)});
        n->smk = gsmk(n->l) + n->x + gsmk(n->r);

        n->mnv = min({gmnv(n->l), n->val, gmnv(n->r)});
        n->mxv = max({gmxv(n->l), n->val, gmxv(n->r)});
        n->smv = gsmv(n->l) + n->val + gsmv(n->r);

        n->sz = gsz(n->l) + 1 + gsz(n->r);
    }

    Node* merge(Node *l, Node *r) {
        if (!l || !r) return l ? l : r;
        if (l->y > r->y) {
            push(l);
            l->r = merge(l->r, r); upd(l);
            return l;
        }
        push(r);
        r->l = merge(l, r->l); upd(r);
        return r;
    }

    pair<Node*, Node*> splitSz(Node *n, ll k) {
        if (!n) return {0, 0};
        push(n);
        if (k <= gsz(n->l)) {
            pair<Node*, Node*> p = splitSz(n->l, k);
            n->l = p.S; upd(n);
            p.S = n;
            return p;
        }
        pair<Node*, Node*> p = splitSz(n->r, k - gsz(n->l) - 1);
        n->r = p.F; upd(n);
        p.F = n;
        return p;
    }

    pair<Node*, Node*> splitKey(Node *n, K k) {
        if (!n) return {0, 0};
        push(n);
        if (k < n->x) {
            pair<Node*, Node*> p = splitKey(n->l, k);
            n->l = p.S; upd(n);
            p.S = n;
            return p;
        }
        pair<Node*, Node*> p = splitKey(n->r, k);
        n->r = p.F; upd(n);
        p.F = n;
        return p;
    }

    template<typename I>
    Node* build(I f_key, I l_key) {
        if (f_key >= l_key) return 0;
        I md = f_key + (l_key - f_key) / 2;
        Node *n = new Node(*md);
        n->l = build(f_key, md);
        n->r = build(md + 1, l_key);
        upd(n);
        return n;
    }

    template<typename I1, typename I2>
    Node* build(I1 f_key, I1 l_key, I2 f_val, I2 l_val) {
        if (f_key >= l_key) return 0;
        I1 md1 = f_key + (l_key - f_key) / 2;
        I2 md2 = f_val + (l_val - f_val) / 2;
        Node *n = new Node(*md1, *md2);
        n->l = build(f_key, md1, f_val, md2);
        n->r = build(md1 + 1, l_key, md2 + 1, l_val);
        upd(n);
        return n;
    }

    void update_val_at_pos(Node *n, int pos, V new_val) {
        push(n);
        if (pos == gsz(n->l)) {n->val = new_val; return;}
        if (pos < gsz(n->l)) update_val_at_pos(n->l, pos, new_val);
        else update_val_at_pos(n->r, pos - gsz(n->l) - 1, new_val);
        upd(n);
    }

    Node* erase_pos(Node *n, ll pos) {
        push(n);
        if (gsz(n->l) == pos) {
            last_erased_key = n->x;
            last_erased_val = n->val;
            Node *l = n->l, *r = n->r;
            delete n;
            return merge(l, r);
        }
        if (pos < gsz(n->l)) n->l = erase_pos(n->l, pos);
        else n->r = erase_pos(n->r, pos - gsz(n->l) - 1);
        upd(n);
        return n;
    }

    Node* insert_node(Node *n, Node *nw) {
        push(n);
        if (!n || nw->y > n->y) {
            pair<Node*, Node*> p = splitKey(n, nw->x);
            nw->l = p.F;
            nw->r = p.S;
            upd(nw);
            return nw;
        }
        if (nw->x < n->x) n->l = insert_node(n->l, nw);
        else n->r = insert_node(n->r, nw);
        upd(n);
        return n;
    }

    Node* erase_one_key(Node *n, K x) {
        push(n);
        if (!n) return 0;
        if (n->x == x) {
            Node *l = n->l, *r = n->r;
            delete n;
            return merge(l, r);
        }
        if (x < n->x) n->l = erase_one_key(n->l, x);
        else n->r = erase_one_key(n->r, x);
        upd(n);
        return n;
    }

    void get_keys_on_subsegment(Node *n, int l, int &len, vec<K> &res) {
        if (!n || len <= 0) return;
        push(n);
        if (l < gsz(n->l)) get_keys_on_subsegment(n -> l, l, len, res);
        if (l <= gsz(n->l) && len) res.pb(n->x), --len;
        get_keys_on_subsegment(n->r, max(0LL, l - gsz(n->l) - 1), len, res);
    }

    int pos_of_leftest_min_key(Node *n) {
        assert(n);
        K mnk = gmnk(n);
        int ans = 0;
        for (; n;) {
            push(n);
            if (gmnk(n->l) == mnk) n = n->l;
            else if (n->x == mnk) return ans + gsz(n->l);
            else {
                ans += gsz(n->l) + 1;
                n = n->r;
            }
        }
        assert(0);
        return ans;
    }

    void print_keys(Node *n) {if (!n) return; push(n); print_keys(n->l); cout << n->x << " "; print_keys(n->r);}
    void print_vals(Node *n) {if (!n) return; push(n); print_vals(n->l); cout << n->val << " "; print_vals(n->r);}

    void delete_subtree(Node *n) {
        if (!n) return;
        push(n);
        delete_subtree(n->l);
        delete_subtree(n->r);
        delete n;
    }

public:
    treap() = default;
    ~treap() {delete_subtree(root);}

    template<typename I> treap(I f_key, I l_key) {root = build(f_key, l_key);}
    template<typename I1, typename I2> treap(I1 f_key, I1 l_key, I2 f_val, I2 l_val) {assert(l_key - f_key == l_val - f_val); root = build(f_key, l_key, f_val, l_val);}

    pair<K, V> operator[](ll pos) {Node *n = root; assert(0 <= pos && pos < gsz(n)); for (;;) {push(n); const int szl = gsz(n->l); if (pos == szl) return {n->x, n->val}; if (pos < szl) n = n->l; else {pos -= szl + 1; n = n->r;}} assert(0); return { -1, -1};}

    ll count_keys_leq(K x) {Node* n = root; ll o = 0; while (n) {push(n); if (n->x <= x) o += gsz(n->l) + 1, n = n->r; else n = n->l;} return o;}
    ll count_keys_less(K x) {Node* n = root; ll o = 0; while (n) {push(n); if (n->x < x) o += gsz(n->l) + 1, n = n->r; else n = n->l;} return o;}
    ll count_keys_in_seg(K l, K r) {return l > r ? 0 : count_keys_leq(r) - count_keys_less(l);}
    ll count_keys_geq(K x) {return gsz(root) - count_keys_less(x);}
    ll count_keys_greater(K x) {return gsz(root) - count_keys_leq(x);}

    int pos_of_leftest_min_key() {return pos_of_leftest_min_key(root);}

    ll size() const {return gsz(root);}
    bool empty() const {return root == 0;}

    void get_keys_on_subsegment(int l, int len, vec<K> &res) {get_keys_on_subsegment(root, l, len, res);}

    void erase_pos(ll pos) {root = erase_pos(root, pos);}
    K extract_pos_get_key(ll pos) {erase_pos(pos); return last_erased_key;}
    V extract_pos_get_val(ll pos) {erase_pos(pos); return last_erased_val;}
    void erase_one_key(K x) {root = erase_one_key(root, x);}
    void remove_seg(int l, int len) {pair<Node*, Node*> p1 = splitSz(root, l); pair<Node*, Node*> p2 = splitSz(p1.S, len); root = merge(p1.F, p2.S);}

    template<typename I> void insert_array(int pos, I first, I last) {pair<Node*, Node*> p = splitSz(root, pos); root = merge(merge(p.F, build(first, last)), p.S);}
    void insert_array_at_pos(int pos, vec<K> arr) {insert_array(pos, all(arr));}
    void insert_elem_at_pos(int pos, K x, V val = UNDEF) {pair<Node*, Node*> p = splitSz(root, pos); root = merge(merge(p.F, new Node(x, val)), p.S);}
    void insert_key(K x, V val = UNDEF) {root = insert_node(root, new Node(x, val));}
    void update_val_at_pos(int pos, V new_val) {update_val_at_pos(root, pos, new_val);}

    void print_keys(str _end = "") {print_keys(root); cout << _end;}
    void print_vals(str _end = "") {print_vals(root); cout << _end;}
};