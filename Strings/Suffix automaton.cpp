template<const int ALPHABET = 26>
struct SuffixAutomaton {
    struct Node {
        Node *m[ALPHABET] = {0};
        Node *sl = 0, *p = 0;
        ll cnt = 0;
        ll dep = 0;

        Node() = default;
        Node(Node *p): p(p), dep(p->dep + 1) {}
    };

    ll diff_substr = 0;
    Node *lst, *root;

    SuffixAutomaton() {
        lst = new Node();
        lst->cnt = 1;
        root = lst;
    }

    void add_char(char c) {
        c -= 'a'; assert(0 <= c && c < ALPHABET);
        Node *y = new Node(lst);
        y->sl = root;
        for (Node *u = lst; u; u = u->sl) {
            if (!u->m[c]) {
                u->m[c] = y;
                y->cnt += u->cnt;
                diff_substr += u->cnt;
                continue;
            }
            Node *j = u->m[c];
            if (j->p == u) {
                y->sl = j;
                break;
            }
            Node *nw = new Node(u);
            nw->sl = j->sl;
            j->sl = nw;
            y->sl = nw;
            memcpy(nw->m, j->m, sizeof(nw->m));
            ll e = 0;
            for (Node *t = u; t && t->m[c] == j; t = t->sl) {
                e += t->cnt;
                t->m[c] = nw;
            }
            nw->cnt = e;
            j->cnt -= e;
            break;
        }
        lst = y->p->m[c];
    }
};