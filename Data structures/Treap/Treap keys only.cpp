template<typename K>
class treap {
    struct Node {
        Node* l = 0;
        Node* r = 0;
        int y;
        size_t sz;

        K key;
        K mnk;
        K mxk;
        K smk;

        Node(K k): y(rnd()), sz(1) {
            key = k;
            mnk = k;
            mxk = k;
            smk = k;
        }
    };
    Node* root = 0;

    K last_erased_key;

    K gmnk(Node* n) const {return n ? n->mnk : std::numeric_limits<K>::max();}
    K gmxk(Node* n) const {return n ? n->mxk : std::numeric_limits<K>::min();}
    K gsmk(Node* n) const {return n ? n->smk : 0;}

    size_t gsz(Node* n) const {return n ? n->sz : 0;}

    //Write, if need
    void apply_push(Node* n) {
        if (!n) return;
    }
    void push(Node* n) {
        if (!n) return;
    }

    void upd(Node* n) {
        if (!n) return;
        n->mnk = min({gmnk(n->l), n->key, gmnk(n->r)});
        n->mxk = max({gmxk(n->l), n->key, gmxk(n->r)});
        n->smk = gsmk(n->l) + n->key + gsmk(n->r);

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

    array<Node*, 2> split_size(Node* n, size_t k) {
        if (!n) return {0, 0};
        push(n);
        if (k <= gsz(n->l)) {
            array<Node*, 2> p = split_size(n->l, k);
            n->l = p[1]; upd(n);
            p[1] = n;
            return p;
        }
        array<Node*, 2> p = split_size(n->r, k - gsz(n->l) - 1);
        n->r = p[0]; upd(n);
        p[0] = n;
        return p;
    }

    array<Node*, 2> split_key(Node* n, K key) {
        if (!n) return {0, 0};
        push(n);
        if (key < n->key) {
            array<Node*, 2> p = split_key(n->l, key);
            n->l = p[1]; upd(n);
            p[1] = n;
            return p;
        }
        array<Node*, 2> p = split_key(n->r, key);
        n->r = p[0]; upd(n);
        p[0] = n;
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

    void update_key_at_pos(Node* n, size_t pos, K new_key) {
        push(n);
        if (pos == gsz(n->l)) {n->key = new_key; upd(n); return;}
        if (pos < gsz(n->l)) update_key_at_pos(n->l, pos, new_key);
        else update_key_at_pos(n->r, pos - gsz(n->l) - 1, new_key);
        upd(n);
    }

    Node* erase_pos(Node* n, size_t pos) {
        push(n);
        if (gsz(n->l) == pos) {
            last_erased_key = n->key;
            Node* l = n->l, *r = n->r;
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
            auto [lf, rg] = split_key(n, nw->key);
            nw->l = lf;
            nw->r = rg;
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
            Node* l = n->l, *r = n->r;
            delete n;
            return merge(l, r);
        }
        if (key < n->key) n->l = erase_one_key(n->l, key);
        else n->r = erase_one_key(n->r, key);
        upd(n);
        return n;
    }

    void get_keys_on_subsegment(Node* n, size_t l, size_t& len, vector<K>& res) {
        if (!n || !len) return;
        push(n);
        if (l < gsz(n->l)) get_keys_on_subsegment(n->l, l, len, res);
        if (l <= gsz(n->l) && len) res.push_back(n->key), --len;
        if (l > gsz(n->l) + 1) get_keys_on_subsegment(n->r, l - gsz(n->l) - 1, len, res);
    }

    K kth_elem(Node* n, size_t k) {
        assert(0 <= k && k < gsz(n));
        while (n) {
            push(n);
            const size_t szl = gsz(n->l);
            if (k == szl) return n->key;
            if (k < szl) n = n->l;
            else k -= szl + 1, n = n->r;
        }
        assert(0);
        return K();
    }

    size_t pos_of_leftest_min_key(Node* n) {
        assert(n);
        K mnk = gmnk(n);
        size_t ans = 0;
        while (n) {
            push(n);
            if (gmnk(n->l) == mnk) n = n->l;
            else if (n->key == mnk) return ans + gsz(n->l);
            else ans += gsz(n->l) + 1, n = n->r;
        }
        assert(0);
        return ans;
    }

    size_t pos_of_rightest_min_key(Node* n) {
        assert(n);
        K mnk = gmnk(n);
        size_t ans = 0;
        while (n) {
            push(n);
            if (gmnk(n->r) == mnk) ans += gsz(n->l) + 1, n = n->r;
            else if (n->key == mnk) return ans + gsz(n->l);
            else n = n->l;
        }
        assert(0);
        return ans;
    }

    size_t pos_of_leftest_key_leq(Node* n, K key) {
        if (gmnk(n) > key) return gsz(n);
        size_t ans = 0;
        while (n) {
            push(n);
            if (gmnk(n->l) <= key) n = n->l;
            else if (n->key <= key) return ans + gsz(n->l);
            else ans += gsz(n->l) + 1, n = n->r;
        }
        assert(0);
        return ans;
    }

    size_t pos_of_leftest_key_geq(Node* n, K key) {
        if (gmxk(n) < key) return gsz(n);
        size_t ans = 0;
        while (n) {
            push(n);
            if (gmxk(n->l) >= key) n = n->l;
            else if (n->key >= key) return ans + gsz(n->l);
            else ans += gsz(n->l) + 1, n = n->r;
        }
        assert(0);
        return ans;
    }

    size_t pos_of_closest_from_right_key_leq(Node* n, size_t pos, K key) {
        size_t szr = gsz(n);
        if (pos >= szr || gmnk(n) > key) return szr;
        size_t ans = 0, pos_to_ret = szr;
        Node* node_to_go = 0;
        while (n) {
            push(n);
            if (pos >= gsz(n->l)) {
                if (pos == gsz(n->l) && n->key <= key) return ans + gsz(n->l);
                ans += gsz(n->l) + 1;
                pos -= min(pos, gsz(n->l) + 1);
                n = n->r;
            } else {
                if (n->key <= key) pos_to_ret = ans + gsz(n->l), node_to_go = 0;
                else if (gmnk(n->r) <= key) pos_to_ret = ans + gsz(n->l) + 1, node_to_go = n->r;
                n = n->l;
            }
        }
        return pos_to_ret + (node_to_go ? pos_of_leftest_key_leq(node_to_go, key) : 0);
    }

    size_t pos_of_closest_from_right_key_geq(Node* n, size_t pos, K key) {
        size_t szr = gsz(n);
        if (pos >= szr || gmxk(n) < key) return szr;
        size_t ans = 0, pos_to_ret = szr;
        Node* node_to_go = 0;
        while (n) {
            push(n);
            if (pos >= gsz(n->l)) {
                if (pos == gsz(n->l) && n->key >= key) return ans + gsz(n->l);
                ans += gsz(n->l) + 1;
                pos -= min(pos, gsz(n->l) + 1);
                n = n->r;
            } else {
                if (n->key >= key) pos_to_ret = ans + gsz(n->l), node_to_go = 0;
                else if (gmxk(n->r) >= key) pos_to_ret = ans + gsz(n->l) + 1, node_to_go = n->r;
                n = n->l;
            }
        }
        return pos_to_ret + (node_to_go ? pos_of_leftest_key_geq(node_to_go, key) : 0);
    }

    void print_keys(Node* n) {if (!n) return; push(n); print_keys(n->l); cout << n->key << ' '; print_keys(n->r);}

    void delete_subtree(Node* n) {if (!n) return; push(n); delete_subtree(n->l); delete_subtree(n->r); delete n;}

    void cyclic_shift_left(Node*& n, int shift) {if (shift < 0) cyclic_shift_right(-shift); else {if (gsz(n) == 0) return; if (shift >= gsz(n)) shift %= gsz(n); auto [lf, rg] = split_size(n, shift); n = merge(rg, lf);}}
    void cyclic_shift_right(Node*& n, int shift) {if (shift < 0) cyclic_shift_left(-shift); else {if (gsz(n) == 0) return; if (shift >= gsz(n)) shift %= gsz(n); auto [lf, rg] = split_size(n, gsz(n) - shift); n = merge(rg, lf);}}

public:
    treap() = default;
    template<typename I> treap(I f_key, I l_key) {root = build(f_key, l_key);}
    ~treap() {delete_subtree(root);}

    size_t size() const {return gsz(root);}
    bool empty() const {return root == 0;}

    template<typename I> void insert_array_at_pos(size_t pos, I first, I last) {auto [lf, rg] = split_size(root, pos); root = merge(merge(lf, build(first, last)), rg);}
    template<typename T> void insert_array_at_pos(size_t pos, initializer_list<T> il) {auto [lf, rg] = split_size(root, pos); root = merge(merge(lf, build(il.begin(), il.end())), rg);}
    void insert_key_at_pos(size_t pos, K key) {auto [lf, rg] = split_size(root, pos); root = merge(merge(lf, new Node(key)), rg);}
    void insert_key(K key) {root = insert_node(root, new Node(key));}

    void update_key_at_pos(size_t pos, K new_key) {update_key_at_pos(root, pos, new_key);}

    void erase_pos(size_t pos) {root = erase_pos(root, pos);}
    void erase_one_key(K key) {root = erase_one_key(root, key);}
    void erase_seg(size_t l, size_t len) {auto [lf, tmp] = split_size(root, l); auto [md, rg] = split_size(tmp, len); root = merge(lf, rg);}

    K extract_pos(size_t pos) {erase_pos(pos); return last_erased_key;}

    K operator[](size_t pos) {return kth_elem(root, pos);}
    
    bool contains(K key) {Node* n = root; while (n) {push(n); if (key == n->key) return true; n = key < n->key ? n->l : n->r;} return false;}
    size_t get_leftest_pos_of_key(K key) {Node* n = root; size_t pos = 0, o = size(); while (n) {push(n); if (key == n->key) o = min(o, pos + gsz(n->l)), n = n->l; else if (key < n->key) n = n->l; else pos += gsz(n->l) + 1, n = n->r;} assert(o < size() && "No such key"); return o;}

    size_t count_keys_leq(K key) {Node* n = root; size_t o = 0; while (n) {push(n); if (n->key <= key) o += gsz(n->l) + 1, n = n->r; else n = n->l;} return o;}
    size_t count_keys_less(K key) {Node* n = root; size_t o = 0; while (n) {push(n); if (n->key < key) o += gsz(n->l) + 1, n = n->r; else n = n->l;} return o;}
    size_t count_keys_in_seg(K l, K r) {return l > r ? 0 : count_keys_leq(r) - count_keys_less(l);}
    size_t count_keys_geq(K key) {return gsz(root) - count_keys_less(key);}
    size_t count_keys_greater(K key) {return gsz(root) - count_keys_leq(key);}
    size_t count_keys_eq(K key) {return count_keys_leq(key) - count_keys_less(key);}

    K pref_sumkey(size_t p) {Node* n = root; K sm = 0; while (n) {push(n); if (gsz(n->l) == p) return sm + gsmk(n->l) + n->key; if (gsz(n->l) < p) sm += gsmk(n->l) + n->key, p -= gsz(n->l) + 1, n = n->r; else n = n->l;} assert(0); return sm;}
    K seg_sumkey_fast(size_t l, size_t r) {return pref_sumkey(r) - (l ? pref_sumkey(l - 1) : 0);}
    K seg_sumkey_slow(size_t l, size_t r) {auto [lf, tmp] = split_size(root, l); auto [mid, rg] = split_size(tmp, r - l + 1); K ans = gsmk(mid); root = merge(merge(lf, mid), rg); return ans;}

    //If no such pos exists, these functions will return size()
    size_t get_pos_of_leftest_key_leq(K key) {return pos_of_leftest_key_leq(root, key);}
    size_t get_pos_of_leftest_key_geq(K key) {return pos_of_leftest_key_geq(root, key);}
    size_t get_pos_of_closest_from_right_leq(size_t pos, K key) {return pos_of_closest_from_right_key_leq(root, pos, key);}
    size_t get_pos_of_closest_from_right_geq(size_t pos, K key) {return pos_of_closest_from_right_key_geq(root, pos, key);}

    size_t get_pos_of_leftest_min_key() {return pos_of_leftest_min_key(root);}
    size_t get_pos_of_rightest_min_key() {return pos_of_rightest_min_key(root);}

    void cyclic_shift_left(int shift) {cyclic_shift_left(root, shift);}
    void cyclic_shift_right(int shift) {cyclic_shift_right(root, shift);}
    void seg_cyclic_shift_left(size_t l, size_t r, int shift) {auto [lf, tmp] = split_size(root, l); auto [mid, rg] = split_size(tmp, r - l + 1); cyclic_shift_left(mid, shift); root = merge(merge(lf, mid), rg);}
    void seg_cyclic_shift_right(size_t l, size_t r, int shift) {auto [lf, tmp] = split_size(root, l); auto [mid, rg] = split_size(tmp, r - l + 1); cyclic_shift_right(mid, shift); root = merge(merge(lf, mid), rg);}

    void print_keys(string end_string = "") {print_keys(root); cout << end_string;}
    vector<K> get_keys_from_seg(size_t l, size_t len) {vector<K> res; get_keys_on_subsegment(root, l, len, res); return res;}
};
