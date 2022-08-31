struct splay_tree {

    static const ll inf = 1e9 + 7;

    struct Node {
        Node *m[2] = {0}, *p = 0;
        ll x;
        ll mn = inf;
        ll sz = 0;
        ll rev = 0;

        Node() = default;

        Node(ll a) {
            x = a;
            sz = 1;
            mn = a;
        }
    };
    Node* root = 0;

    ll gsz(Node *n) { return n ? n->sz : 0; }
    ll gmn(Node *n) { return n ? n->mn : inf; }

    void push(Node *n) {
        if (!n) return;
        if (n->rev) {
            if (n->m[0]) n->m[0]->rev ^= 1;
            if (n->m[1]) n->m[1]->rev ^= 1;
            swap(n->m[0], n->m[1]);
            n->rev = 0;
        }
    }

    void super_push(Node *n) {
        if (n->p) super_push(n->p);
        push(n);
    }

    void upd(Node *n) {
        if (!n) return;
        n->mn = min({gmn(n->m[0]), n->x, gmn(n->m[1])});
        n->sz = gsz(n->m[0]) + 1 + gsz(n->m[1]);
    }

    int dir(Node *n) {return n->p->m[1] == n;}

    void disconnect(Node *n) {
        if (!n || !n->p) return;
        Node *p = n->p;
        p->m[dir(n)] = 0;
        n->p = 0;
        upd(p);
    }

    void connect(Node *p, Node *n, int d) {
        p->m[d] = n;
        if (n) n->p = p;
        upd(p);
    }

    void rotate(Node *n) {
        if (!n->p) return;
        Node *p = n->p, *g = n->p->p;
        int d = dir(n);
        if (g) {
            connect(g, n, dir(p));
        } else {
            n->p = 0;
        }
        connect(p, n->m[d ^ 1], d);
        connect(n, p, d ^ 1);
    }

    Node* splay(Node *n) {
        if (!n) return 0;
        super_push(n);
        for (; n->p;) {
            if (n->p->p) rotate(dir(n) == dir(n->p) ? n->p : n);
            rotate(n);
        }
        return n;
    }

    Node* leftmost(Node *n) {
        push(n);
        return n->m[0] ? leftmost(n->m[0]) : n;
    }

    Node* rightmost(Node *n) {
        push(n);
        return n->m[1] ? rightmost(n->m[1]) : n;
    }

    Node* merge(Node *l, Node *r) {
        if (!l || !r) return l ? l : r;
        l = splay(rightmost(l));
        connect(l, r, 1);
        return l;
    }

    Node* access(Node *n, int ps) {
        push(n);
        if (ps <= gsz(n->m[0])) return access(n->m[0], ps);
        if (ps > gsz(n->m[0]) + 1) return access(n->m[1], ps - gsz(n->m[0]) - 1);
        splay(n);
        return n;
    }

    pnn splitSz(Node *n, int k) {
        if (!k) return {0, n};
        n = access(n, k);
        Node *R = n->m[1];
        disconnect(R);
        return {n, R};
    }

    ll seg_min(ll l, ll r) {
        pnn p1 = splitSz(root, l - 1), p2 = splitSz(p1.S, r - l + 1);
        ll res = gmn(p2.F);
        root = merge(p1.F, merge(p2.F, p2.S));
        return res;
    }

    void seg_rev(ll l, ll r) {
        pnn p1 = splitSz(root, l - 1), p2 = splitSz(p1.S, r - l + 1);
        p2.F->rev ^= 1;
        root = merge(p1.F, merge(p2.F, p2.S));
    }
};