template<typename K>
class link_cut_tree {
    static constexpr K inf = numeric_limits<K>::max();

    struct Node {
        Node* m[2] = {0, 0};
        Node* p = 0;
        Node* pp = 0;
        size_t num;
        size_t sz;

        K key;
        K mnk;
        K mxk;
        K smk;
        K ps_set_key = inf;
        K ps_add_key = 0;

        bool rev = 0;

        Node() = default;

        Node(K k, size_t num): num(num), sz(1) {
        	key = k;
            mnk = k;
            mxk = k;
            smk = k;
        }
    };

    vector<Node*> store;

    size_t gsz(Node* n) {return n ? n->sz : 0;}
    K gmnk(Node* n) {return n ? n->mnk : inf;}
    K gmxk(Node* n) {return n ? n->mxk : -inf;}
    K gsmk(Node* n) {return n ? n->smk : 0;}

    void apply_add(Node* n, K c) {
        if (!n) return;
        n->key += c;
    	n->mnk += c;
    	n->mxk += c;
    	n->smk += gsz(n) * c;
        (n->ps_set_key != inf ? n->ps_set_key : n->ps_add_key) += c;
    }

    void apply_set(Node* n, K c) {
        if (!n) return;
        n->ps_set_key = c;
        n->ps_add_key = 0;
        n->key = c;
        n->mnk = c;
        n->mxk = c;
        n->smk = gsz(n) * c;
    }

    void push(Node* n) {
        if (!n) return;
        if (n->rev) {
            if (n->m[0]) n->m[0]->rev ^= 1;
            if (n->m[1]) n->m[1]->rev ^= 1;
            swap(n->m[0], n->m[1]);
        	n->rev = 0;
        }
        if (n->ps_add_key) {
            apply_add(n->m[0], n->ps_add_key);
            apply_add(n->m[1], n->ps_add_key);
            n->ps_add_key = 0;
        }
        if (n->ps_set_key != inf) {
            apply_set(n->m[0], n->ps_set_key);
            apply_set(n->m[1], n->ps_set_key);
        	n->ps_set_key = inf;
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
        n->smk = gsmk(n->m[0]) + n->key + gsmk(n->m[1]);
        n->mnk = min({gmnk(n->m[0]), n->key, gmnk(n->m[1])});
        n->mxk = max({gmxk(n->m[0]), n->key, gmxk(n->m[1])});
        n->sz = gsz(n->m[0]) + 1 + gsz(n->m[1]);
    }

    int dir(Node* x) {
        return x->p->m[1] == x;
    }

    void connect(Node* x, Node* par, int d) {
        if (x) x->p = par;
        par->m[d] = x;
        upd(par);
    }

    void disconnect(Node* par, int d) {
        if (par->m[d]) par->m[d]->p = 0;
        par->m[d] = 0;
        upd(par);
    }

    void rotate(Node* x) {
        if (!x->p) return;
        Node* p = x->p;
        Node* g = p->p;
        int d = dir(x);
        if (g) connect(x, g, dir(p));
        else x->p = 0;
        connect(x->m[d ^ 1], p, d);
        connect(p, x, d ^ 1);
    }

    Node* get_root_of_splay_tree(Node* n) {
        while (n->p) n = n->p;
        return n;
    }

    void splay(Node* n) {
        if (!n) return;
        super_push(n);
        if (!n->p) return;
        Node* rt = get_root_of_splay_tree(n);
        while (n->p) {
            if (n->p->p) rotate(dir(n) == dir(n->p) ? n->p : n);
            rotate(n);
        }
        set_path_parent(n, rt->pp);
        del_path_parent(rt);
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
        l = rightmost(l);
        splay(l);
        connect(r, l, 1);
        return l;
    }

    Node* access(Node* n, size_t pos) {
        assert(0 <= pos && pos < gsz(n));
        while (n) {
            push(n);
            const size_t szl = gsz(n->m[0]);
            if (pos == szl) {
                splay(n);
                return n;
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
        disconnect(r, 1);
        return {n, r};
    }

    void seg_rev(Node*& n, size_t l, size_t r) {
        auto [lf, tmp] = split_size(n, l);
        auto [mid, rg] = split_size(tmp, r - l + 1);
        mid->rev ^= 1;
        n = merge(merge(lf, mid), rg);
    }

    void set_path_parent(Node* n, Node* pp) {
        assert(!n->pp);
        n->pp = pp;
    }

    void del_path_parent(Node* n) {
        if (!n->pp) return;
        n->pp = 0;
    }

    void expose(int v) {
        Node* n = store[v];
        splay(n);
        if (n->m[1]) {
            set_path_parent(n->m[1], n);
            disconnect(n, 1);
        }
        while (n->pp) {
            Node* u = n->pp;
            del_path_parent(n);
            splay(u);
            if (u->m[1]) {
                set_path_parent(u->m[1], u);
                disconnect(u, 1);
            }
            connect(n, u, 1);
            splay(n);
        }
    }

public:
    link_cut_tree() = default;
    link_cut_tree(size_t V, vector<K> vals, vector<K> nums) {
        store.resize(V);
        for (size_t i = 0; i < V; ++i) {
            store[i] = new Node(vals[i], nums[i]);
        }
    }

    int get_root_of_LCT(int n) {
        expose(n);
        return access(store[n], 0)->num;
    }

    int is_connected(int x, int y) {
        return get_root_of_LCT(x) == get_root_of_LCT(y);
    }

    int depth(int x) {
        expose(x);
        return gsz(store[x]);
    }

    int root = 0;
    void reroot(int n) {
        expose(n);
        store[n]->rev ^= 1;
        root = n;
    }

    void link(int n, int p) {
        if (is_connected(n, p)) return;
        int old_root = root;
        reroot(n);
        set_path_parent(store[n], store[p]);
        reroot(old_root);
    }

    void cut(int x, int y) {
        if (depth(x) < depth(y)) swap(x, y);
        expose(y);
        splay(store[x]);
        del_path_parent(store[x]);
    }

    void cut(int x) {
        expose(x);
        if (store[x]->m[0]) disconnect(store[x], 0);
    }

    int lca(int x, int y) {
        if (x == y) return x;
        if (depth(x) < depth(y)) swap(x, y);
        expose(x);
        expose(y);
        return store[x]->pp->num;
    }

    void path_set(int x, int y, K val) {
    	int old_root = root;
	    reroot(x);
        expose(y);
        apply_set(store[y], val);
        reroot(old_root);
    }

    void path_add(int x, int y, K val) {
    	int old_root = root;
	    reroot(x);
        expose(y);
        apply_add(store[y], val);
        reroot(old_root);
    }

    K path_sum(int x, int y) {
    	int old_root = root;
    	reroot(x);
        expose(y);
        K res = store[y]->smk;
        reroot(old_root);
        return res;
    }

    K path_min(int x, int y) {
    	int old_root = root;
    	reroot(x);
        expose(y);
        K res = store[y]->mnk;
        reroot(old_root);
        return res;
    }

    K path_max(int x, int y) {
    	int old_root = root;
    	reroot(x);
        expose(y);
        K res = store[y]->mxk;
        reroot(old_root);
        return res;
    }
};
