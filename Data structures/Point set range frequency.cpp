template<typename T>
class point_set_range_frequency {

    class treap {

        struct Node {
            Node* l = 0;
            Node* r = 0;
            size_t y;
            size_t sz;

            size_t k;

            Node() = default;
            Node(size_t k): k(k), sz(1) {
                static mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
                y = rnd();
            }
        };

        Node* root = 0;

        size_t gsz(Node* n) const {return n ? n->sz : 0;}

        void upd(Node* n) {
            if (!n) return;
            n->sz = gsz(n->l) + 1 + gsz(n->r);
        }

        Node* merge(Node* l, Node* r) {
            if (!l || !r) return l ? l : r;
            if (l->y < r->y) {
                l->r = merge(l->r, r);
                upd(l);
                return l;
            }
            r->l = merge(l, r->l);
            upd(r);
            return r;
        }

        array<Node*, 2> split(Node* n, size_t k) {
            if (!n) return {0, 0};
            if (k < n->k) {
                auto [l, r] = split(n->l, k);
                n->l = r;
                upd(n);
                return {l, n};
            }
            auto [l, r] = split(n->r, k);
            n->r = l;
            upd(n);
            return {n, r};
        }

        size_t query_pref(size_t k) const {
            Node* n = root;
            size_t res = 0;
            while (n) {
                if (k == n->k) return res + gsz(n->l) + 1;
                if (k < n->k) n = n->l;
                else res += gsz(n->l) + 1, n = n->r;
            }
            return res;
        }

    public:
        treap() = default;

        void insert(size_t k) {
            static mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
            size_t y = rnd();
            function<Node*(Node*)> go = [&](Node* n) {
                if (!n || y < n->y) {
                    auto [l, r] = split(n, k);
                    Node* res = new Node(k);
                    res->y = y;
                    res->l = l;
                    res->r = r;
                    upd(res);
                    return res;
                }
                (k < n->k ? n->l = go(n->l) : n->r = go(n->r));
                upd(n);
                return n;
            };
            root = go(root);
        }

        void erase(size_t k) {
            function<Node*(Node*)> go = [&](Node* n) -> Node* {
                if (!n) return 0;
                if (k == n->k) {
                    Node* res = merge(n->l, n->r);
                    delete n;
                    return res;
                }
                (k < n->k ? n->l = go(n->l) : n->r = go(n->r));
                upd(n);
                return n;
            };
            root = go(root);
        }

        size_t query(size_t l, size_t r) const {
            return query_pref(r) - (l ? query_pref(l - 1) : 0);
        }
    };

    size_t n;
    vector<T> m;
    map<T, treap> mp;

public:
    point_set_range_frequency() = default;

    //<O(nlogn), O(n)>
    template<typename I>
    point_set_range_frequency(I first, I last): n(std::distance(first, last)), m(first, last) {
        for (size_t i = 0; i < m.size(); ++i) {
            mp[m[i]].insert(i);
        }
    }

    //O(log(n))
    void point_set(size_t i, T x) {
        assert(i < n);
        mp[m[i]].erase(i);
        m[i] = x;
        mp[m[i]].insert(i);
    }

    //O(log(n))
    size_t seg_frequency(size_t l, size_t r, T x) const {
        assert(l <= r && r < n);
        return mp.count(x) ? mp.at(x).query(l, r) : 0;
    }
};
