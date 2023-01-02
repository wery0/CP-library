template<typename K, typename V>
class treap {
    static const V UNDEF = 0;
    struct Node {
        Node* l = 0;
        Node* r = 0;
        ll y;
        ll sz;

        K key;
        K mnk;
        K mxk;
        K smk;

        V val;
        V mnv;
        V mxv;
        V smv;

        Node(K k, V v = UNDEF): y(rnd()), sz(1) {
            key = k;
            mnk = k;
            mxk = k;
            smk = k;

            val = v;
            mnv = v;
            mxv = v;
            smv = v;
        }
    };
    Node* root = 0;

    K last_erased_key;
    V last_erased_val;

    K gmnk(Node* n) const {return n ? n->mnk : std::numeric_limits<K>::max();}
    K gmxk(Node* n) const {return n ? n->mxk : std::numeric_limits<K>::min();}
    K gsmk(Node* n) const {return n ? n->smk : 0;}

    V gmnv(Node* n) const {return n ? n->mnv : std::numeric_limits<V>::max();}
    V gmxv(Node* n) const {return n ? n->mxv : std::numeric_limits<V>::min();}
    V gsmv(Node* n) const {return n ? n->smv : 0;}

    ll gsz(Node* n) const {return n ? n->sz : 0;}

    //Write, if need
    inline void apply_push(Node* n) {
        if (!n) return;
    }
    inline void push(Node* n) {
        if (!n) return;
    }

    inline void upd(Node* n) {
        if (!n) return;
        n->mnk = min({gmnk(n->l), n->key, gmnk(n->r)});
        n->mxk = max({gmxk(n->l), n->key, gmxk(n->r)});
        n->smk = gsmk(n->l) + n->key + gsmk(n->r);

        n->mnv = min({gmnv(n->l), n->val, gmnv(n->r)});
        n->mxv = max({gmxv(n->l), n->val, gmxv(n->r)});
        n->smv = gsmv(n->l) + n->val + gsmv(n->r);

        n->sz = gsz(n->l) + 1 + gsz(n->r);
    }

    Node* merge(Node* l, Node* r) {
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

    pair<Node*, Node*> split_size(Node* n, ll key) {
        if (!n) return {0, 0};
        push(n);
        if (key <= gsz(n->l)) {
            pair<Node*, Node*> p = split_size(n->l, key);
            n->l = p.S; upd(n);
            p.S = n;
            return p;
        }
        pair<Node*, Node*> p = split_size(n->r, key - gsz(n->l) - 1);
        n->r = p.F; upd(n);
        p.F = n;
        return p;
    }

    pair<Node*, Node*> split_key(Node* n, K key) {
        if (!n) return {0, 0};
        push(n);
        if (key < n->key) {
            pair<Node*, Node*> p = split_key(n->l, key);
            n->l = p.S; upd(n);
            p.S = n;
            return p;
        }
        pair<Node*, Node*> p = split_key(n->r, key);
        n->r = p.F; upd(n);
        p.F = n;
        return p;
    }

    template<typename I>
    Node* build(I f_key, I l_key) {
        if (f_key >= l_key) return 0;
        I md = f_key + (l_key - f_key) / 2;
        Node* n = new Node(*md);
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
        Node* n = new Node(*md1, *md2);
        n->l = build(f_key, md1, f_val, md2);
        n->r = build(md1 + 1, l_key, md2 + 1, l_val);
        upd(n);
        return n;
    }

    void update_val_at_pos(Node* n, int pos, V new_val) {
        push(n);
        if (pos == gsz(n->l)) {n->val = new_val; upd(n); return;}
        if (pos < gsz(n->l)) update_val_at_pos(n->l, pos, new_val);
        else update_val_at_pos(n->r, pos - gsz(n->l) - 1, new_val);
        upd(n);
    }

    Node* erase_pos(Node* n, ll pos) {
        push(n);
        if (gsz(n->l) == pos) {
            last_erased_key = n->key;
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

    Node* insert_node(Node* n, Node* nw) {
        push(n);
        if (!n || nw->y > n->y) {
            pair<Node*, Node*> p = split_key(n, nw->key);
            nw->l = p.F;
            nw->r = p.S;
            upd(nw);
            return nw;
        }
        if (nw->key < n->key) n->l = insert_node(n->l, nw);
        else n->r = insert_node(n->r, nw);
        upd(n);
        return n;
    }

    Node* erase_one_key(Node* n, K key) {
        if (!n) return 0;
        push(n);
        if (n->key == key) {
            Node *l = n->l, *r = n->r;
            delete n;
            return merge(l, r);
        }
        if (key < n->key) n->l = erase_one_key(n->l, key);
        else n->r = erase_one_key(n->r, key);
        upd(n);
        return n;
    }

    void get_keys_on_subsegment(Node* n, int l, int& len, vector<K>& res) {
        if (!n || len <= 0) return;
        push(n);
        if (l < gsz(n->l)) get_keys_on_subsegment(n->l, l, len, res);
        if (l <= gsz(n->l) && len) res.push_back(n->key), --len;
        get_keys_on_subsegment(n->r, max(0LL, l - gsz(n->l) - 1), len, res);
    }

    int pos_of_leftest_min_key(Node* n) {
        assert(n);
        K mnk = gmnk(n);
        int ans = 0;
        for (; n;) {
            push(n);
            if (gmnk(n->l) == mnk) n = n->l;
            else if (n->key == mnk) return ans + gsz(n->l);
            else ans += gsz(n->l) + 1, n = n->r;
        }
        assert(0);
        return ans;
    }

    void print_keys(Node* n) {if (!n) return; push(n); print_keys(n->l); cout << n->key << ' '; print_keys(n->r);}
    void print_vals(Node* n) {if (!n) return; push(n); print_vals(n->l); cout << n->val << ' '; print_vals(n->r);}

    void delete_subtree(Node* n) {
        if (!n) return;
        push(n);
        delete_subtree(n->l);
        delete_subtree(n->r);
        delete n;
    }

public:
    treap() = default;
    template<typename I> treap(I f_key, I l_key) {root = build(f_key, l_key);}
    template<typename I1, typename I2> treap(I1 f_key, I1 l_key, I2 f_val, I2 l_val) {assert(l_key - f_key == l_val - f_val); root = build(f_key, l_key, f_val, l_val);}
    ~treap() {delete_subtree(root);}

    ll size() const {return gsz(root);}
    bool empty() const {return root == 0;}

    template<typename I>
    void insert_array_at_pos(int pos, I first, I last) {pair<Node*, Node*> p = split_size(root, pos); root = merge(merge(p.F, build(first, last)), p.S);}
    void insert_key_at_pos(int pos, K key, V val = UNDEF) {pair<Node*, Node*> p = split_size(root, pos); root = merge(merge(p.F, new Node(key, val)), p.S);}
    void insert_key(K key, V val = UNDEF) {root = insert_node(root, new Node(key, val));}
    void update_val_at_pos(int pos, V new_val) {update_val_at_pos(root, pos, new_val);}

    void erase_pos(ll pos) {root = erase_pos(root, pos);}
    void erase_one_key(K key) {root = erase_one_key(root, key);}
    void erase_seg(int l, int len) {pair<Node*, Node*> p1 = split_size(root, l); pair<Node*, Node*> p2 = split_size(p1.S, len); root = merge(p1.F, p2.S);}

    K extract_pos_get_key(ll pos) {erase_pos(pos); return last_erased_key;}
    V extract_pos_get_val(ll pos) {erase_pos(pos); return last_erased_val;}
    pair<K, V> extract_pos(ll pos) {erase_pos(pos); return {last_erased_key, last_erased_val};}

    pair<K, V> operator[](ll pos) {Node* n = root; assert(0 <= pos && pos < gsz(n)); for (;;) {push(n); const int szl = gsz(n->l); if (pos == szl) return {n->key, n->val}; if (pos < szl) n = n->l; else {pos -= szl + 1; n = n->r;}} assert(0); return { -1, -1};}
    V get_value_by_key(K key) {Node* n = root; while (n) {push(n); if (n->key == key) return n->val; if (key < n->key) n = n->l; else n = n->r;} assert(0); return 0;}
    ll get_leftest_pos_of_key(K key) {Node* n = root; ll pos = 0, o = size; while (n) {push(n); if (key == n->x) o = min(o, pos + gsz(n->l)), n = n->l; else if (key < n->x) n = n->l; else pos += gsz(n->l) + 1, n = n->r;} assert(o < size()); return o;}

    ll count_keys_leq(K key) {Node* n = root; ll o = 0; while (n) {push(n); if (n->key <= key) o += gsz(n->l) + 1, n = n->r; else n = n->l;} return o;}
    ll count_keys_less(K key) {Node* n = root; ll o = 0; while (n) {push(n); if (n->key < key) o += gsz(n->l) + 1, n = n->r; else n = n->l;} return o;}
    ll count_keys_in_seg(K l, K r) {return l > r ? 0 : count_keys_leq(r) - count_keys_less(l);}
    ll count_keys_geq(K key) {return gsz(root) - count_keys_less(key);}
    ll count_keys_greater(K key) {return gsz(root) - count_keys_leq(key);}
    ll count_keys_eq(K key) {return count_keys_leq(key) - count_keys_less(key);}

    K pref_sum(ll p) {Node* n = root; K sm = 0; while (n) {push(n); if (gsz(n->l) == p) return sm + gsmk(n->l) + n->key; if (gsz(n->l) < p) sm += gsmk(n->l) + n->key, p -= gsz(n->l) + 1, n = n->r; else n = n->l;} assert(0); return sm;}
    K seg_sum_fast(ll l, ll r) {return pref_sum(r) - (l ? pref_sum(l - 1) : 0);}
    K seg_sum_slow(int l, int r) {
        auto [lf, tmp] = split_size(root, l);
        auto [mid, rg] = split_size(tmp, r - l + 1);
        K ans = gsmk(mid);
        root = merge(merge(lf, mid), rg);
        return ans;
    }

    int get_pos_of_leftest_min_key() {return pos_of_leftest_min_key(root);}
    vector<K> get_keys_from_seg(int l, int len) {vector<K> res; get_keys_on_subsegment(root, l, len, res); return res;}

    void print_keys(string end_string = "") {print_keys(root); cout << end_string;}
    void print_vals(string end_string = "") {print_vals(root); cout << end_string;}
};