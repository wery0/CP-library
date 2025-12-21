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

        Node(const Node* rhs) {
            assert(rhs);
            l = rhs->l ? new Node(rhs->l) : 0;
            r = rhs->r ? new Node(rhs->r) : 0;
            y = rhs->y;
            sz = rhs->sz;

            key = rhs->key;
            mnk = rhs->mnk;
            mxk = rhs->mxk;
            smk = rhs->smk;
        }

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
        // if (!n || !n->?) return;
        // apply_push(n->l, n->?);
        // apply_push(n->r, n->?);
        // n->? = 0;
    }

    void upd(Node* n) {
        if (!n) return;
        n->mnk = min({gmnk(n->l), n->key, gmnk(n->r)});
        n->mxk = max({gmxk(n->l), n->key, gmxk(n->r)});
        n->smk = gsmk(n->l) + n->key + gsmk(n->r);

        n->sz = gsz(n->l) + 1 + gsz(n->r);
    }

    void _bump(Node*& n) {
        //Slower, but less memory
        Node* l = n->l, *r = n->r;
        delete n; n = merge(l, r);

        //Faster, but more memory
        //n = merge(n->l, n->r);
    }

    void _copy_from(const treap& rhs) {
        root = rhs.root ? new Node(rhs.root) : 0;
        last_erased_key = rhs.last_erased_key;
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
    Node* _build(I f_key, I l_key) {
        if (f_key >= l_key) return 0;
        I md = f_key + (l_key - f_key) / 2;
        Node* n = new Node(*md);
        n->l = _build(f_key, md);
        n->r = _build(md + 1, l_key);
        upd(n);
        return n;
    }

    void _delete_subtree(Node* n) {if (!n) return; push(n); _delete_subtree(n->l); _delete_subtree(n->r); delete n;}

    void _update_key_at_pos(Node* n, size_t pos, K new_key) {
        push(n);
        if (pos == gsz(n->l)) {n->key = new_key; upd(n); return;}
        if (pos < gsz(n->l)) _update_key_at_pos(n->l, pos, new_key);
        else _update_key_at_pos(n->r, pos - gsz(n->l) - 1, new_key);
        upd(n);
    }

    K _kth_elem(Node* n, size_t k) {
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

    void _get_keys_from_seg(Node* n, size_t l, size_t& len, vector<K>& res) {
        if (!n || !len) return;
        push(n);
        if (l < gsz(n->l) && len) _get_keys_from_seg(n->l, l, len, res);
        if (l <= gsz(n->l) && len) res.push_back(n->key), --len;
        if (len) _get_keys_from_seg(n->r, l > gsz(n->l) ? l - gsz(n->l) - 1 : 0, len, res);
    }

    size_t _pos_of_leftest_good(Node* n, auto has_good, auto is_good) {
        size_t ans = 0;
        while (n) {
            push(n);
            if (has_good(n->l)) n = n->l;
            else if (is_good(n)) return ans + gsz(n->l);
            else ans += gsz(n->l) + 1, n = n->r;
        }
        return ans;
    }

    size_t _pos_of_rightest_good(Node* n, auto has_good, auto is_good) {
        size_t ans = 0;
        while (n) {
            push(n);
            if (has_good(n->r)) ans += gsz(n->l) + 1, n = n->r;
            else if (is_good(n)) return ans + gsz(n->l);
            else n = n->l;
        }
        return ans;
    }

    size_t _pos_of_closest_from_right_good(Node* n, size_t pos, auto has_good, auto is_good) {
        size_t szr = gsz(n);
        if (pos >= szr || !has_good(n)) return szr;
        size_t ans = 0, pos_to_ret = szr;
        Node* u = 0;
        while (n) {
            push(n);
            if (pos >= gsz(n->l)) {
                if (pos == gsz(n->l) && is_good(n)) return ans + gsz(n->l);
                ans += gsz(n->l) + 1;
                pos -= min(pos, gsz(n->l) + 1);
                n = n->r;
            } else {
                if (is_good(n)) pos_to_ret = ans + gsz(n->l), u = 0;
                else if (has_good(n->r)) pos_to_ret = ans + gsz(n->l) + 1, u = n->r;
                n = n->l;
            }
        }
        return pos_to_ret + (u ? _pos_of_leftest_good(u, has_good, is_good) : 0);
    }

    size_t _pos_of_closest_from_left_good(Node* n, size_t pos, auto has_good, auto is_good) {
        size_t szr = gsz(n);
        if (!has_good(n)) return szr;
        pos = min(pos, szr - 1);
        pos = szr - 1 - pos;
        size_t ans = 0, pos_to_ret = szr;
        Node* u = 0;
        while (n) {
            push(n);
            if (pos >= gsz(n->r)) {
                if (pos == gsz(n->r) && is_good(n)) return szr - 1 - (ans + gsz(n->r));
                ans += gsz(n->r) + 1;
                pos -= min(pos, gsz(n->r) + 1);
                n = n->l;
            } else {
                if (is_good(n)) pos_to_ret = ans + gsz(n->r), u = 0;
                else if (has_good(n->l)) pos_to_ret = ans + gsz(n->r) + 1, u = n->l;
                n = n->r;
            }
        }
        if (pos_to_ret == szr) return szr;
        pos_to_ret = szr - 1 - pos_to_ret;
        return pos_to_ret - (u ? gsz(u) - 1 - _pos_of_rightest_good(u, has_good, is_good) : 0);
    }

    size_t _pos_of_leftest_min_key(Node* n) {
        K mnk = gmnk(n);
        return _pos_of_leftest_good(n, 
            [&](Node* n){return gmnk(n) == mnk;},
            [&](Node* n){return n->key == mnk;});
    }

    size_t _pos_of_rightest_min_key(Node* n) {
        K mnk = gmnk(n);
        return _pos_of_rightest_good(n, 
            [&](Node* n){return gmnk(n) == mnk;},
            [&](Node* n){return n->key == mnk;});
    }

    size_t _pos_of_leftest_key_leq(Node* n, K key) {
        return _pos_of_leftest_good(n, 
            [&](Node* n){return gmnk(n) <= key;},
            [&](Node* n){return n->key <= key;});
    }

    size_t _pos_of_closest_from_left_key_leq(Node* n, size_t pos, K key) {
        return _pos_of_closest_from_left_good(n, pos,
            [&](Node* n){return gmnk(n) <= key;},
            [&](Node* n){return n->key <= key;});
    }

    size_t _pos_of_closest_from_right_key_leq(Node* n, size_t pos, K key) {
        return _pos_of_closest_from_right_good(n, pos,
            [&](Node* n){return gmnk(n) <= key;},
            [&](Node* n){return n->key <= key;});
    }

    void _insert(Node*& n, Node* nw) {
        push(n);
        if (!n || nw->y > n->y) {
            auto [lf, rg] = split_key(n, nw->key);
            nw->l = lf;
            nw->r = rg;
            upd(nw);
            n = nw;
            return;
        }
        if (nw->key < n->key) _insert(n->l, nw);
        else _insert(n->r, nw);
        upd(n);
    }

    void _erase_pos(Node*& n, size_t pos) {
        push(n);
        if (gsz(n->l) == pos) {
            last_erased_key = n->key;
            _bump(n);
            return;
        }
        (pos < gsz(n->l) ? _erase_pos(n->l, pos) : _erase_pos(n->r, pos - gsz(n->l) - 1));
        upd(n);
    }

    void _erase_one_key_occurrence(Node*& n, K key) {
        if (!n) return;
        push(n);
        if (n->key == key) {
            _bump(n);
            return;
        }
        _erase_one_key_occurrence(key < n->key ? n->l : n->r, key);
        upd(n);
    }

    void _erase_all_key_occurrences(Node*& n, K key) {
        if (!n) return;
        push(n);
        if (n->key == key) {
            _erase_all_key_occurrences(n->l, key);
            _erase_all_key_occurrences(n->r, key);
            _bump(n);
            return;
        }
        _erase_all_key_occurrences(key < n->key ? n->l : n->r, key);
        upd(n);
    }

    void _erase_seg(Node*& n, size_t l, size_t len) {auto [lf, tmp] = split_size(n, l); auto [md, rg] = split_size(tmp, len); n = merge(lf, rg);}
    void _erase_back(Node*& n) {_erase_pos(n, gsz(n) - 1);}

    template<typename I> void _insert_array_at_pos(Node* n, size_t pos, I first, I last) {auto [lf, rg] = split_size(n, pos); n = merge(merge(lf, _build(first, last)), rg);}
    template<typename T> void _insert_array_at_pos(Node* n, size_t pos, initializer_list<T> il) {auto [lf, rg] = split_size(n, pos); n = merge(merge(lf, _build(il.begin(), il.end())), rg);}
    void _insert_at_pos(Node*& n, size_t pos, K key) {auto [lf, rg] = split_size(n, pos); n = merge(merge(lf, new Node(key)), rg);}
    void _insert_back(Node*& n, K key) {_insert_at_pos(n, gsz(n), key);}

    bool _contains(Node* n, K key) {while (n) {push(n); if (key == n->key) return true; n = key < n->key ? n->l : n->r;} return false;}
    size_t _get_leftest_pos_of_key(Node* n, K key) {size_t pos = 0, o = size(); while (n) {push(n); if (key == n->key) o = min(o, pos + gsz(n->l)), n = n->l; else if (key < n->key) n = n->l; else pos += gsz(n->l) + 1, n = n->r;} assert(o < size() && "No such key"); return o;}

    size_t _count_keys_leq(Node* n, K key) {size_t o = 0; while (n) {push(n); if (n->key <= key) o += gsz(n->l) + 1, n = n->r; else n = n->l;} return o;}
    size_t _count_keys_less(Node* n, K key) {size_t o = 0; while (n) {push(n); if (n->key < key) o += gsz(n->l) + 1, n = n->r; else n = n->l;} return o;}
    size_t _count_keys_in_seg(Node* n, K l, K r) {return l > r ? 0 : _count_keys_leq(n, r) - _count_keys_less(n, l);}
    size_t _count_keys_geq(Node* n, K key) {return gsz(n) - _count_keys_less(n, key);}
    size_t _count_keys_greater(Node* n, K key) {return gsz(n) - _count_keys_leq(n, key);}
    size_t _count_keys_eq(Node* n, K key) {return _count_keys_leq(n, key) - _count_keys_less(n, key);}

    K _pref_sumkey(Node* n, size_t p) {K sm = 0; while (n) {push(n); if (gsz(n->l) == p) return sm + gsmk(n->l) + n->key; if (gsz(n->l) < p) sm += gsmk(n->l) + n->key, p -= gsz(n->l) + 1, n = n->r; else n = n->l;} assert(0); return sm;}
    K _seg_sumkey_fast(Node* n, size_t l, size_t r) {return _pref_sumkey(n, r) - (l ? _pref_sumkey(n, l - 1) : 0);}
    K _seg_sumkey_slow(Node* n, size_t l, size_t r) {auto [lf, tmp] = split_size(n, l); auto [md, rg] = split_size(tmp, r - l + 1); K ans = gsmk(md); n = merge(merge(lf, md), rg); return ans;}

    void _cyclic_shift_left(Node*& n, int shift) {if (shift < 0) _cyclic_shift_right(n, -shift); else {if (gsz(n) == 0) return; if (shift >= gsz(n)) shift %= gsz(n); auto [lf, rg] = split_size(n, shift); n = merge(rg, lf);}}
    void _cyclic_shift_right(Node*& n, int shift) {if (shift < 0) _cyclic_shift_left(n, -shift); else {if (gsz(n) == 0) return; if (shift >= gsz(n)) shift %= gsz(n); auto [lf, rg] = split_size(n, gsz(n) - shift); n = merge(rg, lf);}}
    void _seg_cyclic_shift_left(Node*& n, size_t l, size_t r, int shift) {auto [lf, tmp] = split_size(n, l); auto [md, rg] = split_size(tmp, r - l + 1); _cyclic_shift_left(md, shift); n = merge(merge(lf, md), rg);}
    void _seg_cyclic_shift_right(Node*& n, size_t l, size_t r, int shift) {auto [lf, tmp] = split_size(n, l); auto [md, rg] = split_size(tmp, r - l + 1); _cyclic_shift_right(md, shift); n = merge(merge(lf, md), rg);}

    void _print_keys(Node* n) {if (!n) return; push(n); _print_keys(n->l); cout << n->key << ' '; _print_keys(n->r);}

public:
    treap() = default;
    template<typename I> treap(I f_key, I l_key) {root = _build(f_key, l_key);}
    treap(const treap& rhs) {_copy_from(rhs);}
    treap& operator=(const treap& rhs) {_delete_subtree(root); _copy_from(rhs); return *this;}
    ~treap() {_delete_subtree(root);}

    size_t size() const {return gsz(root);}
    bool empty() const {return root == 0;}

    template<typename I> void insert_array_at_pos(size_t pos, I first, I last) {_insert_array_at_pos(root, pos, first, last);}
    template<typename T> void insert_array_at_pos(size_t pos, initializer_list<T> il) {_insert_array_at_pos(root, pos, il);}
    void insert(K key) {_insert(root, new Node(key));}
    void insert_at_pos(size_t pos, K key) {_insert_at_pos(root, pos, key);}
    void insert_back(K key) {_insert_back(root, key);}

    void update_key_at_pos(size_t pos, K new_key) {_update_key_at_pos(root, pos, new_key);}

    void erase_pos(size_t pos) {_erase_pos(root, pos);}
    void erase_one_key_occurrence(K key) {_erase_one_key_occurrence(root, key);}
    void erase_all_key_occurrences(K key) {_erase_all_key_occurrences(root, key);}
    void erase_seg(size_t l, size_t len) {_erase_seg(root, l, len);}
    void erase_back() {_erase_pos(root, size() - 1);}

    K extract_pos_get_key(size_t pos) {erase_pos(pos); return last_erased_key;}

    bool contains(K key) {return _contains(root, key);}
    size_t get_leftest_pos_of_key(K key) {return _get_leftest_pos_of_key(root, key);}
    K operator[](size_t pos) {return _kth_elem(root, pos);}

    size_t count_keys_leq(K key) {return _count_keys_leq(root, key);}
    size_t count_keys_less(K key) {return _count_keys_less(root, key);}
    size_t count_keys_in_seg(K l, K r) {return _count_keys_in_seg(root, l, r);}
    size_t count_keys_geq(K key) {return _count_keys_geq(root, key);}
    size_t count_keys_greater(K key) {return _count_keys_greater(root, key);}
    size_t count_keys_eq(K key) {return _count_keys_eq(root, key);}

    K pref_sumkey(size_t p) {return _pref_sumkey(root, p);}
    K seg_sumkey_fast(size_t l, size_t r) {return _seg_sumkey_fast(root, l, r);}
    K seg_sumkey_slow(size_t l, size_t r) {return _seg_sumkey_slow(root, l, r);}

    //If no such pos exists, these functions will return size()
    size_t pos_of_leftest_key_leq(K key) {return _pos_of_leftest_key_leq(root, key);}
    size_t pos_of_closest_from_left_key_leq(size_t pos, K key) {return _pos_of_closest_from_left_key_leq(root, pos, key);}
    size_t pos_of_closest_from_right_key_leq(size_t pos, K key) {return _pos_of_closest_from_right_key_leq(root, pos, key);}

    size_t pos_of_leftest_min_key() {return _pos_of_leftest_min_key(root);}
    size_t pos_of_rightest_min_key() {return _pos_of_rightest_min_key(root);}

    void cyclic_shift_left(int shift) {_cyclic_shift_left(root, shift);}
    void cyclic_shift_right(int shift) {_cyclic_shift_right(root, shift);}
    void seg_cyclic_shift_left(size_t l, size_t r, int shift) {_seg_cyclic_shift_left(root, l, r, shift);}
    void seg_cyclic_shift_right(size_t l, size_t r, int shift) {_seg_cyclic_shift_right(root, l, r, shift);}

    void print_keys(string end_string = "") {_print_keys(root); cout << end_string;}
    vector<K> get_keys_from_seg(size_t l, size_t len) {vector<K> res; _get_keys_from_seg(root, l, len, res); return res;}
};
