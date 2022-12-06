struct aho_corasick {

    //Change, if need
    static constexpr int ALPHABET = 26;
    static constexpr int FIRST_CHAR = 'a';

    aho_corasick() = default;

    struct Node {
        Node* parent = 0;
        Node* suf_link = 0;
        Node* compressed_suf_link = 0;
        int parent_edge_char = -1;
        map<int, Node*> m;
        int is_terminal = 0;

        vector<int> who_ends;
        int dep = 0;

        Node(Node *parent, int chr): parent(parent), parent_edge_char(chr) {
            dep = (parent ? parent->dep : -1) + 1;
        }
    };

    Node* root = new Node(0, -1);
    int added_strings = 0;
    bool was_suf_links_built = false;

    Node* go(Node* n, int chr) {
        if (n->m.count(chr)) return n->m[chr];
        return n->m[chr] = n->parent ? go(n->suf_link, chr) : n;
    }

    void build_suf_links() {
        assert(!was_suf_links_built);
        deque<Node*> dq = {root};
        for (; dq.size();) {
            Node *n = dq.front();
            dq.pop_front();
            if (!n->parent) {
                n->suf_link = n->compressed_suf_link = n;
            } else if (!n->parent->parent) {
                n->suf_link = n->compressed_suf_link = n->parent;
            } else {
                n->suf_link = go(n->parent->suf_link, n->parent_edge_char);
                n->compressed_suf_link = n->suf_link->is_terminal ? n->suf_link : n->suf_link->compressed_suf_link;
            }
            for (auto &[chr, u] : n->m) {
                dq.push_back(u);
            }
        }
        was_suf_links_built = true;
    }

    void add_string(string &t) {
        assert(!was_suf_links_built);
        Node* n = root;
        for (int q = 0; q < t.size(); ++q) {
            int chr = t[q] - FIRST_CHAR;
            if (!n->m.count(chr)) n->m[chr] = new Node(n, chr);
            n = n->m[chr];
        }
        n->is_terminal = 1;
        n->who_ends.push_back(added_strings++);
    }

    //For every added string returns a vector with all occurences in s.
    /*
        Example:
        s = "abacaba"
        "a" -> {0, 2, 4, 6}
        "aba" -> {0, 4}
    */
    //O(#occurences)
    vector<vector<int>> get_all_occurences(string &s) {
        assert(was_suf_links_built);
        Node *n = root;
        vector<vector<int>> ans(added_strings);
        for (int q = 0; q < s.size(); q++) {
            n = go(n, s[q] - FIRST_CHAR);
            for (Node *e = n; e->parent; e = e->compressed_suf_link) {
                for (int i : e->who_ends) {
                    ans[i].push_back(q + 1 - e->dep);
                }
            }
        }
        return ans;
    }
};