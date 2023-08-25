class suffix_automaton {

	//Change, if need
	static constexpr int ALPHABET = 26;
	static constexpr int FIRST_CHAR = 'a';

    struct Node {
        Node *m[ALPHABET] = {0};
        Node *suf_link = 0, *parent = 0;
        int64_t cnt = 0;
        int64_t dep = 0;

        Node() = default;
        Node(Node *parent): parent(parent), dep(parent->dep + 1) {}
    };

    int64_t distinct_substrings = 0;
    Node *lst, *root;

public:
    suffix_automaton() = default;

    suffix_automaton() {
        lst = new Node();
        lst->cnt = 1;
        root = lst;
    }

    void add_char(char c) {
        c -= FIRST_CHAR; assert(0 <= c && c < ALPHABET);
        Node* y = new Node(lst);
        y->suf_link = root;
        for (Node* u = lst; u; u = u->suf_link) {
            if (!u->m[c]) {
                u->m[c] = y;
                y->cnt += u->cnt;
                distinct_substrings += u->cnt;
                continue;
            }
            Node* j = u->m[c];
            if (j->parent == u) {
                y->suf_link = j;
                break;
            }
            Node* nw = new Node(u);
            nw->suf_link = j->suf_link;
            j->suf_link = nw;
            y->suf_link = nw;
            memcpy(nw->m, j->m, sizeof(nw->m));
            int64_t e = 0;
            for (Node* t = u; t && t->m[c] == j; t = t->suf_link) {
                e += t->cnt;
                t->m[c] = nw;
            }
            nw->cnt = e;
            j->cnt -= e;
            break;
        }
        lst = y->parent->m[c];
    }
};
