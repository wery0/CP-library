enum STORAGE {ARRAY, MAP, UNORDERED_MAP};
template<const int ALPHABET, const int FIRST_CHAR, const bool are_strings_distinct, STORAGE storage = ARRAY>
class aho_corasick {

    struct Node;
    using K = conditional_t<are_strings_distinct, int, vector<int>>;
    using M = conditional_t<storage == ARRAY, Node*[ALPHABET], conditional_t<storage == MAP, map<int, Node*>, unordered_map<int, Node*>>>;

    struct Node {
        Node* parent = 0;
        Node* suf_link = 0;
        Node* compressed_suf_link = 0;
        int parent_edge_char = -1;
        M m = {};

        Node() {
            if constexpr(storage == ARRAY) {
                memset(m, 0, sizeof(Node*) * ALPHABET);
            }
        }

        K who_ends;
        int dep = 0;

        Node(Node* parent, int chr): parent(parent), parent_edge_char(chr) {
            if constexpr(are_strings_distinct) who_ends = -1;
            dep = (parent ? parent->dep : -1) + 1;
        }
    };

    Node* root = new Node(0, -1);
    int cnt_added_strings = 0;
    int cnt_nodes = 1;
    int cnt_different_strings = 0;
    bool is_prepared = false;

    inline bool has(Node* n, int chr) const {
        if constexpr(storage == ARRAY) return n->m[chr];
        else return n->m.count(chr);
    }

    inline bool is_terminal(Node* n) const {
        if constexpr(are_strings_distinct) return n->who_ends != -1;
        else return !n->who_ends.empty();
    }

    Node* go(Node* n, int chr) {
        if (has(n, chr)) return n->m[chr];
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
                n->compressed_suf_link = is_terminal(n->suf_link) ? n->suf_link : n->suf_link->compressed_suf_link;
            }
            if constexpr(storage == ARRAY) {
                for (size_t i = 0; i < ALPHABET; ++i) {
                    if (n->m[i]) dq.push_back(n->m[i]);
                }
            } else {
                for (auto& [chr, u] : n->m) {
                    dq.push_back(u);
                }
            }
        }
        is_prepared = true;
    }

public:
    aho_corasick() = default;

    void add_string(const string& s) {
        assert(!is_prepared);
        assert(s.size());
        Node* n = root;
        for (int chr : s) {
            chr -= FIRST_CHAR;
            assert(0 <= chr < ALPHABET);
            if (!has(n, chr)) n->m[chr] = new Node(n, chr), ++cnt_nodes;
            n = n->m[chr];
        }
        cnt_different_strings += !is_terminal(n);
        if constexpr(are_strings_distinct) {
            assert(n->who_ends == -1 && "Strings should be different!");
            n->who_ends = cnt_added_strings++;
        } else {
            n->who_ends.push_back(cnt_added_strings++);
        }
    }

    void prepare() {
        build_suf_links();
    }

    //For every added string returns a vector with all occurences (positions where it starts) in s.
    /*
        Example:
        s = "abacaba"
        "a" -> {0, 2, 4, 6}
        "aba" -> {0, 4}
    */
    //<O(|text| + #occurences), O(#occurences)>)
    vector<vector<int>> get_for_each_string_where_it_starts_in_text(const string& text) {
        assert(is_prepared);
        Node* n = root;
        vector<vector<int>> ans(cnt_added_strings);
        for (size_t i = 0; i < text.size(); ++i) {
            n = go(n, text[i] - FIRST_CHAR);
            for (Node* e = n; e->parent; e = e->compressed_suf_link) {
                if constexpr(are_strings_distinct) {
                    if (e->who_ends != -1) ans[e->who_ends].push_back(i + 1 - e->dep);
                } else {
                    for (int j : e->who_ends) {
                        ans[j].push_back(i + 1 - e->dep);
                    }
                }
            }
        }
        return ans;
    }

    struct occurrences_iterable {
        Node* n;

        class occurrences_iterator {
            Node* n;
            int i = 0;

            void move() {
                if constexpr(are_strings_distinct) {
                    while (n && n->who_ends == -1) n = n->compressed_suf_link == n ? n->parent : n->compressed_suf_link;
                } else {
                    while (n && i == n->who_ends.size()) {
                        i = 0;
                        n = n->compressed_suf_link == n ? n->parent : n->compressed_suf_link;
                    }
                }
            }

        public:
            occurrences_iterator(Node* n): n(n) {
                move();
            };

            occurrences_iterator& operator++() {
                if constexpr(are_strings_distinct) {
                    n = n->compressed_suf_link;
                } else {
                    ++i;
                    assert(n && i <= n->who_ends.size());
                }
                move();
                return *this;
            }

            int operator*() {
                if constexpr(are_strings_distinct) return n->who_ends;
                else return n->who_ends[i];
            }

            bool operator==(const occurrences_iterator& rhs) const {
                if constexpr(are_strings_distinct) return n == rhs.n;
                else return n == rhs.n && i == rhs.i;
            }
        };

        occurrences_iterable() = default;
        occurrences_iterable(Node* n): n(n) {};

        auto begin() {return occurrences_iterator(n);}
        auto begin() const {return occurrences_iterator(n);}
        auto end() {return occurrences_iterator(nullptr);}
        auto end() const {return occurrences_iterator(nullptr);}
    };

    //Returns a vector of iterable objects, ith of which iterates through all strings that end in position i.
    /*
        Example:
        .add_string("a");
        .add_string("aba");
        .prepare();
        .get_who_ends_in_each_position_of_text("abacaba") == 
        [
            [0],
            [],
            [1, 0],
            [],
            [0],
            [],
            [1, 0]
        ]
    */
    //<O(|text|), O(|text|)>)
    vector<occurrences_iterable> get_who_ends_in_each_position_of_text(const string& text) {
        vector<occurrences_iterable> ans(text.size());
        Node* n = root;
        for (size_t i = 0; i < text.size(); ++i) {
            n = go(n, text[i] - FIRST_CHAR);
            ans[i] = occurrences_iterable(n);
        }
        return ans;
    }
};
//Example of initialization: aho_corasick<26, 'a', true, UNORDERED_MAP> ac;
