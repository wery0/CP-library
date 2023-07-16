class aho_corasick {

    //Change, if need
    static constexpr int ALPHABET = 26;
    static constexpr int FIRST_CHAR = 'a';

    struct Node {
        Node* parent = 0;
        Node* suf_link = 0;
        Node* compressed_suf_link = 0;
        int parent_edge_char = -1;
        map<int, Node*> m;
        int is_terminal = 0;

        vector<int> who_ends;
        int dep = 0;

        Node(Node* parent, int chr): parent(parent), parent_edge_char(chr) {
            dep = (parent ? parent->dep : -1) + 1;
        }
    };

    Node* root = new Node(0, -1);
    int added_strings = 0;
    bool is_prepared = false;

    Node* go(Node* n, int chr) {
        if (n->m.count(chr)) return n->m[chr];
        return n->m[chr] = n->parent ? go(n->suf_link, chr) : n;
    }

    void build_suf_links() {
        assert(!is_prepared);
        deque<Node*> dq = {root};
        while (dq.size()) {
            Node* n = dq.front();
            dq.pop_front();
            if (!n->parent) {
                n->suf_link = n->compressed_suf_link = n;
            } else if (!n->parent->parent) {
                n->suf_link = n->compressed_suf_link = n->parent;
            } else {
                n->suf_link = go(n->parent->suf_link, n->parent_edge_char);
                n->compressed_suf_link = n->suf_link->is_terminal ? n->suf_link : n->suf_link->compressed_suf_link;
            }
            for (auto& [chr, u] : n->m) {
                dq.push_back(u);
            }
        }
        is_prepared = true;
    }

public:
    aho_corasick() = default;

    void add_string(const string& t) {
        assert(!is_prepared);
        Node* n = root;
        for (int i = 0; i < t.size(); ++i) {
            int chr = t[i] - FIRST_CHAR;
            if (!n->m.count(chr)) n->m[chr] = new Node(n, chr);
            n = n->m[chr];
        }
        n->is_terminal = 1;
        n->who_ends.push_back(added_strings++);
    }

    void prepare() {
        build_suf_links();
    }

    //For every added string returns a vector with all occurences in s.
    /*
        Example:
        s = "abacaba"
        "a" -> {0, 2, 4, 6}
        "aba" -> {0, 4}
    */
    //O(#occurences)
    vector<vector<int>> get_all_occurences(const string& s) {
        assert(is_prepared);
        Node* n = root;
        vector<vector<int>> ans(added_strings);
        for (int i = 0; i < s.size(); ++i) {
            n = go(n, s[i] - FIRST_CHAR);
            for (Node* e = n; e->parent; e = e->compressed_suf_link) {
                for (int j : e->who_ends) {
                    ans[j].push_back(i + 1 - e->dep);
                }
            }
        }
        return ans;
    }
};