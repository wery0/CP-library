template<typename T, bool is_multiset, typename C>
class intset_mergeable {
    static constexpr T INF_T = numeric_limits<T>::max();
    struct Node {
        Node* l = 0;
        Node* r = 0;
        C sz = 0;

        Node() = default;
        Node(const Node* rhs) {
            assert(rhs);
            l = rhs->l ? new Node(rhs->l) : 0;
            r = rhs->r ? new Node(rhs->r) : 0;
            sz = rhs->sz;
        }
    };
    Node* root = new Node();
    T N = 2;

    size_t gsz(Node* n) const {return n ? n->sz : 0;}
    void upd(Node* n) {
        if (!n) return;
        n->sz = gsz(n->l) + gsz(n->r);
    }

    void delete_subtree(Node* n) {
        if (!n) return;
        delete_subtree(n->l);
        delete_subtree(n->r);
        delete n;
    }

    void _copy_from(const intset_mergeable& rhs) {
        root = new Node(rhs.root);
        N = rhs.N;
    }

    void increase_height() {
        if (gsz(root)) {
            Node* nroot = new Node();
            nroot->l = root;
            nroot->sz = gsz(root);
            root = nroot;
        }
        N *= 2;
    }

public:
    intset_mergeable() = default;
    intset_mergeable(const intset_mergeable& rhs) {_copy_from(rhs);}
    intset_mergeable& operator=(const intset_mergeable& rhs) {_delete_subtree(root); _copy_from(rhs); return *this;}
    ~intset_mergeable() {delete_subtree(root);}

    C size() const {return gsz(root);}

    void insert(T x, C cnt = 1) {
        assert(x >= 0 && cnt >= 0);
        while (x >= N) increase_height();
        function<void(T, T, Node* n)> go = [&](T l, T r, Node * n) {
            if (l == r) {
                if constexpr (is_multiset) {
                    n->sz += cnt;
                } else {
                    n->sz = 1;
                }
                return;
            }
            T m = l + (r - l) / 2;
            if (x <= m) {
                if (!n->l) n->l = new Node();
                go(l, m, n->l);
            } else {
                if (!n->r) n->r = new Node();
                go(m + 1, r, n->r);
            }
            upd(n);
        };
        go(0, N - 1, root);
    }

    void erase(T x, C cnt = 1) {
        assert(x >= 0 && cnt >= 0);
        if (x >= N) return;
        function<void(T, T, Node* n)> go = [&](T l, T r, Node * n) {
            if (!n) return;
            if (l == r) {
                if constexpr (is_multiset) {
                    n->sz = cnt >= n->sz ? 0 : n->sz - cnt;
                } else {
                    n->sz = 0;
                }
                return;
            }
            T m = l + (r - l) / 2;
            if (x <= m) {
                go(l, m, n->l);
                if (!gsz(n->l)) {delete n->l; n->l = 0;}
            } else {
                go(m + 1, r, n->r);
                if (!gsz(n->r)) {delete n->r; n->r = 0;}
            }
            upd(n);
        };
        go(0, N - 1, root);
    }

    C count(T x) const {
        assert(x >= 0);
        if (x >= N) return 0;
        Node* n = root;
        T l = 0, r = N - 1;
        for (; l < r;) {
            T m = l + (r - l) / 2;
            if (x <= m) {
                if (!n->l) return 0;
                n = n->l;
                r = m;
            } else {
                if (!n->r) return 0;
                n = n->r;
                l = m + 1;
            }
        }
        return n->sz;
    }

    C count_less(T x) const {
        if (x <= 0) return 0;
        if (x >= N) return gsz(root);
        Node* n = root;
        T l = 0, r = N - 1;
        C res = 0;
        for (; l < r && n;) {
            T m = l + (r - l) / 2;
            if (x <= m) {
                n = n->l;
                r = m;
            } else {
                res += gsz(n->l);
                n = n->r;
                l = m + 1;
            }
        }
        return res;
    }
    C count_leq(T x) const {return count_less(x + 1);}

    bool contains(T x) const {return count(x);}

    T operator[](C pos) const {
        assert(0 <= pos && pos < gsz(root));
        Node* n = root; T l = 0, r = N - 1;
        for (; l < r;) {
            T m = l + (r - l) / 2;
            if (pos < gsz(n->l)) {
                n = n->l;
                r = m;
            } else {
                pos -= gsz(n->l);
                n = n->r;
                l = m + 1;
            }
        }
        return l;
    }

    T lower_bound(T x) const {
        if (x >= N) return INF_T;
        Node* n = root; T l = 0, r = N - 1;
        Node* lst_r = 0;
        for (; l < r && n;) {
            T m = l + (r - l) / 2;
            if (x <= m) {
                if (gsz(n->r)) lst_r = n->r;
                n = n->l;
                r = m;
            } else {
                n = n->r;
                l = m + 1;
            }
        }
        if (n) return l;
        if (!lst_r) return INF_T;
        for (; l < r;) {
            T m = l + (r - l) / 2;
            if (n->l) n = n->l, r = m;
            else n = n->r, l = m + 1;
        }
        assert(n);
        return l;
    }

    void merge(intset_mergeable<T, is_multiset, C>& rhs) {
        while (N < rhs.N) increase_height();
        while (rhs.N < N) rhs.increase_height();
        assert(N == rhs.N);
        function<Node*(T, T, Node*, Node*)> go = [&](T l, T r, Node * nl, Node * nr) {
            if (!nl || !nr) return nl ? nl : nr;
            if (l == r) {
                if constexpr (is_multiset) nl->sz += nr->sz;
                else nl->sz = 1;
            } else {
                T m = l + (r - l) / 2;
                nl->l = go(l, m, nl->l, nr->l);
                nl->r = go(m + 1, r, nl->r, nr->r);
                upd(nl);
            }
            delete nr;
            return nl;
        };
        root = go(0, N - 1, root, rhs.root);
        rhs.root = 0;
    }

    void split_size(C k, intset_mergeable<T, is_multiset, C>& rhs) {
        assert(0 <= k && k <= gsz(root));
        rhs.N = N;
        if (k == 0) {
            rhs.root->l = root->l, rhs.root->r = root->r;
            rhs.root->sz = root->sz;
            root->l = 0, root->r = 0, root->sz = 0;
            return;
        }
        function<void(T, T, C, Node*, Node*)> go = [&](T l, T r, C k, Node * nl, Node * nr) {
            assert(nl && nr);
            if (l == r) {
                assert(is_multiset);
                assert(0 < k);
                assert(k < nl->sz);
                nr->sz = nl->sz - k;
                nl->sz = k;
                return;
            }
            T m = l + (r - l) / 2;
            if (k < gsz(nl->l)) {
                nr->r = nl->r;
                nl->r = 0;
                if (!nr->l) nr->l = new Node();
                go(l, m, k, nl->l, nr->l);
            } else if (k == gsz(nl->l)) {
                nr->r = nl->r;
                nl->r = 0;
            } else {
                if (!nr->r) nr->r = new Node();
                go(m + 1, r, k - gsz(nl->l), nl->r, nr->r);
            }
            upd(nl);
            upd(nr);
        };
        go(0, N - 1, k, root, rhs.root);
    }

    void split_key(T k, intset_mergeable<T, is_multiset, C>& rhs) {
        split_size(count_leq(k), rhs);
    }

    void split_key_fast(T k, intset_mergeable<T, is_multiset, C>& rhs) {
        rhs.N = N;
        function<void(T, T, C, Node*, Node*)> go = [&](T l, T r, C k, Node * nl, Node * nr) {
            assert(nl && nr);
            if (l == r) {
                if (l <= k);
                else nr->sz = nl->sz, nl->sz = 0;
                return;
            }
            T m = l + (r - l) / 2;
            if (k <= m) {
                nr->r = nl->r;
                nl->r = 0;
                if (nl->l) {
                    if (!nr->l) nr->l = new Node();
                    go(l, m, k, nl->l, nr->l);
                }
            } else {
                if (nl->r) {
                    if (!nr->r) nr->r = new Node();
                    go(m + 1, r, k, nl->r, nr->r);
                }
            }
            upd(nl);
            upd(nr);
            if (nl->l && !gsz(nl->l)) delete nl->l, nl->l = 0;
            if (nl->r && !gsz(nl->r)) delete nl->r, nl->r = 0;
            if (nr->l && !gsz(nr->l)) delete nr->l, nr->l = 0;
            if (nr->r && !gsz(nr->r)) delete nr->r, nr->r = 0;
        };
        go(0, N - 1, k, root, rhs.root);
    }

    vector<T> get_values() const {
        vector<T> res; res.reserve(gsz(root));
        function<void(T, T, Node* n)> go = [&](T l, T r, Node* n) {
            if (!n) return;
            if (l == r) {
                if constexpr (is_multiset) {
                    for (C i = 0; i < n->sz; ++i) res.push_back(l);
                } else {
                    assert(n->sz == 1);
                    res.push_back(l);
                }
                return;
            }
            T m = l + (r - l) / 2;
            go(l, m, n->l);
            go(m + 1, r, n->r);
        };
        go(0, N - 1, root);
        return res;
    }
};
