//This structure supports online addition and deletion of non-intersecting diagonals between points of n-gon
template<typename T, bool allow_common_vertexes>
class ngon_diagonals {
    static_assert(is_integral_v<T>);

    using K = array<T, 2>;
    struct Node {
        Node* l = 0;
        Node* r = 0;
        int y;

        K key;
        T mn;
        T mx;

        Node(K k): y(rnd()) {
            key = k;
            mn = k[1];
            mx = k[1];
        }
    };
    Node* root = 0;

    T gmn(Node* n) const {return n ? n->mn : std::numeric_limits<T>::max();}
    T gmx(Node* n) const {return n ? n->mx : std::numeric_limits<T>::min();}

    void upd(Node* n) {
        if (!n) return;
        n->mn = min({gmn(n->l), n->key[1], gmn(n->r)});
        n->mx = max({gmx(n->l), n->key[1], gmx(n->r)});
    }

    Node* merge(Node* l, Node* r) {
        if (!l || !r) return l ? l : r;
        if (l->y > r->y) {
            l->r = merge(l->r, r); upd(l);
            return l;
        }
        r->l = merge(l, r->l); upd(r);
        return r;
    }

    array<Node*, 2> split_key(Node* n, K key) {
        if (!n) return {0, 0};
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

    Node* insert_node(Node* n, Node* nw) {
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

    Node* erase_one_key_occurrence(Node* n, K key) {
        if (!n) return 0;
        if (n->key == key) {
            Node* l = n->l, *r = n->r;
            delete n;
            return merge(l, r);
        }
        if (key < n->key) n->l = erase_one_key_occurrence(n->l, key);
        else n->r = erase_one_key_occurrence(n->r, key);
        upd(n);
        return n;
    }

    void delete_subtree(Node* n) {if (!n) return; push(n); delete_subtree(n->l); delete_subtree(n->r); delete n;}
    void insert(K key) {root = insert_node(root, new Node(key));}
    void erase_one_key_occurrence(K key) {root = erase_one_key_occurrence(root, key);}

    bool can(T l, T r) {
        auto [lf, tmp] = split_key(root, {l, numeric_limits<T>::min()}); auto [mid, rg] = split_key(tmp, {r, numeric_limits<T>::max()});
        T mn = gmn(mid), mx = gmx(mid);
        bool res = !mid || (allow_common_vertexes ? l <= mn && mx <= r : l < mn && mx < r);
        root = merge(merge(lf, mid), rg);
        return res;
    }

    T n;

public:
    ngon_diagonals() = default;
    ngon_diagonals(T n): n(n) {}

    void clear() {
        delete_subtree(root);
    }

    //O(log(|edges|))
    bool link(T x, T y) {
        if (x > y) swap(x, y);
        assert(0 <= x && x != y && y < n);
        if (!can(x, y)) return false;
        insert({x, y});
        insert({y, x});
        return true;
    }

    //O(log(|edges|))
    void unlink(T x, T y) {
        if (x > y) swap(x, y);
        assert(0 <= x && x != y && y < n);
        erase_one_key_occurrence({x, y});
        erase_one_key_occurrence({y, x});
    }
};
