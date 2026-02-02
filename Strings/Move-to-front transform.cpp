namespace MTF {
    template<typename K>
    class treap {
        struct Node {
            Node* l = 0; Node* r = 0;
            int y, sz; K key;
            Node(K k): y(rnd()), sz(1) {key = k;}
        };
        Node* root = 0;
        K last_erased_key;
        int gsz(Node* n) const {return n ? n->sz : 0;}
        void upd(Node* n) {if (!n) return; n->sz = gsz(n->l) + 1 + gsz(n->r);}
        void _bump(Node*& n) {Node* l = n->l, *r = n->r; delete n; n = merge(l, r);}
        Node* merge(Node* l, Node* r) {
            if (!l || !r) return l ? l : r;
            if (l->y > r->y) {l->r = merge(l->r, r); upd(l); return l;}
            r->l = merge(l, r->l); upd(r);
            return r;
        }
        array<Node*, 2> split_size(Node* n, int k) {
            if (!n) return {0, 0};
            if (k <= gsz(n->l)) {
                array<Node*, 2> p = split_size(n->l, k);
                n->l = p[1]; upd(n); p[1] = n; return p;
            }
            array<Node*, 2> p = split_size(n->r, k - gsz(n->l) - 1);
            n->r = p[0]; upd(n); p[0] = n; return p;
        }
        Node* _build(int f_key, int l_key) {
            if (f_key >= l_key) return 0;
            int md = f_key + (l_key - f_key) / 2;
            Node* n = new Node(md);
            n->l = _build(f_key, md);
            n->r = _build(md + 1, l_key);
            upd(n); return n;
        }
        void _delete_subtree(Node* n) {if (!n) return; _delete_subtree(n->l); _delete_subtree(n->r); delete n;}
        void _erase_pos(Node*& n, int pos) {
            if (gsz(n->l) == pos) {last_erased_key = n->key; _bump(n); return;}
            (pos < gsz(n->l) ? _erase_pos(n->l, pos) : _erase_pos(n->r, pos - gsz(n->l) - 1));
            upd(n);
        }
        int _extract_key_get_pos(Node*& n, K key) {
            if (!n) assert(0 && "No such key");
            if (n->key == key) {int res = gsz(n->l); _bump(n); return res;}
            --n->sz;
            if (key < n->key) return _extract_key_get_pos(n->l, key);
            return gsz(n->l) + 1 + _extract_key_get_pos(n->r, key);
        }
    public:
        treap(int n) {root = _build(0, n);}
        ~treap() {_delete_subtree(root);}
        int size() const {return gsz(root);}
        void insert_front(K key) {root = merge(new Node(key), root);}
        K extract_pos_get_key(int pos) {_erase_pos(root, pos); return last_erased_key;}
        int extract_key_get_pos(K key) {return _extract_key_get_pos(root, key);}
    };
}

//Move-to-front (MTF) transform
//Complexity: <O(nlog(distinct characters)), O(n)>
template<typename T_arr>
pair<vector<int>, vector<typename T_arr::value_type>> mtf_encode(T_arr data) {
    using T = T_arr::value_type;
    const int n = data.size();
    vector<T> items(data.begin(), data.end());
    sort(items.begin(), items.end());
    items.erase(unique(items.begin(), items.end()), items.end());
    map<T, int> mp;
    for (int i = 0; const auto& item : items) mp[item] = i++;
    MTF::treap<int> t(mp.size());
    vector<int> encoded(n);
    for (int i = 0; i < n; ++i) {
        int& k = mp[data[i]];
        encoded[i] = t.extract_key_get_pos(k);
        k = -i - 1;
        t.insert_front(-i - 1);
    }
    return {encoded, items};
}

//Move-to-front (MTF) inverse transform
//Complexity: <O(nlog(distinct characters)), O(n)>
template<typename T_arr>
T_arr mtf_decode(vector<int> encoded, vector<typename T_arr::value_type> items) {
    using T = T_arr::value_type;
    const int n = encoded.size();
    MTF::treap<int> t(items.size());
    T_arr decoded(n, T());
    for (int i = 0; i < n; ++i) {
        int idx = t.extract_pos_get_key(encoded[i]);
        decoded[i] = items[idx];
        t.insert_front(idx);
    }
    return decoded;
}
