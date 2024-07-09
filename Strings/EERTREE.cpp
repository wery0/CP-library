template<const int APLHABET, const int FIRST_CHAR>
class EERTREE {
    
    struct Node {
        int len = 0;
        Node* sl = 0;
        Node* m[ALPHABET] = {0};

        Node() = default;
        Node(int d): len(d) {}
    };

    string t;
    Node* r0 = new Node(0);
    Node* r1 = new Node(-1);
    Node* lst = r0;

    Node* go(Node* n) {
        for (; n != r1; n = n->sl) {
            int cur = t.size() - 1;
            int pr = cur - 1 - n->len;
            if (pr >= 0 && t[pr] == t[cur]) break;
        }
        return n;
    }

    void destroy(Node* n) {
        if (!n) return;
        for (auto c : n->m) destroy(c);
        delete n;
    }

public:
    int vertex_count = 0;

    EERTREE() {r0->sl = r1->sl = r1;}

    void add_char(char c) {
        assert(FIRST_CHAR <= c && c < FIRST_CHAR + ALPHABET);
        t += c;
        c -= FIRST_CHAR;
        lst = go(lst);
        if (!lst->m[c]) {
            ++vertex_count;
            lst->m[c] = new Node(lst->len + 2);
            lst->m[c]->sl = lst == r1 ? r0 : go(lst->sl)->m[c];
        }
        lst = lst->m[c];
    }

    void clear() {
        destroy(r0), destroy(r1);
        r0 = new Node(0), r1 = new Node(-1);
        r0->sl = r1->sl = r1;
        lst = 0;
        vertex_count = 0;
        t.clear();
    }

    string get_string() const {return t;}
};
