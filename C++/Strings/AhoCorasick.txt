struct AhoCorasick {
    static constexpr int ALPHABET = 26;
    static constexpr int FIRST_CHAR = 'a';

    AhoCorasick() {}

    struct Node {
        Node *p = 0, *sl = 0, *csl = 0;
        int c = -1;
        map<int, Node*> m;
        int is_terminal = 0;

        vec<int> who_ends;
        int dep = 0;

        Node(Node *P, int _c) {
            p = P;
            c = _c;
            dep = (P ? P->dep : -1) + 1;
        }
    };

    Node* root = new Node(0, -1);
    int added_strings = 0;

    Node *go(Node *n, int c) {
        if (n->m.count(c)) return n->m[c];
        return n->m[c] = n->p ? go(n->sl, c) : n;
    }

    void build_links() {
        deque<Node*> dq = {root};
        for (; dq.size();) {
            Node *n = dq.front();
            dq.pop_front();
            if (!n->p) {
                n->sl = n->csl = n;
            } else if (!n->p->p) {
                n->sl = n->csl = n->p;
            } else {
                n->sl = go(n->p->sl, n->c);
                n->csl = n->sl->is_terminal ? n->sl : n->sl->csl;
            }
            for (auto &[c, u] : n->m) {
                dq.pb(u);
            }
        }
    }

    void add(str &t) {
        Node *n = root;
        for (int q = 0; q < t.size(); ++q) {
            int c = t[q] - FIRST_CHAR;
            if (!n->m.count(c)) n->m[c] = new Node(n, c);
            n = n->m[c];
        }
        n->is_terminal = 1;
        n->who_ends.pb(added_strings++);
    }

    //for every added string returns a vector with all occurences in s.
    vec<vec<int>> get_all_occurences(str &s) {
        Node *n = root;
        vec<vec<int>> ans(added_strings);
        for (int q = 0; q < s.size(); q++) {
            n = go(n, s[q] - FIRST_CHAR);
            Node *e = n;
            for (; e->p; e = e->csl) {
                for (int i : e->who_ends) {
                    ans[i].pb(q + 1 - e->dep);
                }
            }
        }
        return ans;
    }
};