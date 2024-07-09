enum STORAGE {ARRAY, MAP, UNORDERED_MAP};
template<const int APLHABET, const int FIRST_CHAR, const bool are_strings_distinct, STORAGE storage = ARRAY>
class trie {

    struct Node;
    using K = conditional_t<are_strings_distinct, int, vector<int>>;
    using M = conditional_t<storage == ARRAY, Node*[ALPHABET], conditional_t<storage == MAP, map<int, Node*>, unordered_map<int, Node*>>>;
    using T = int;
    static constexpr T INF = numeric_limits<T>::max();

    struct Node {
        Node* parent = 0;
        int parent_edge_char = -1;
        M m = {};

        Node() {
            if constexpr(storage == ARRAY) {
                memset(m, 0, sizeof(Node*) * ALPHABET);
            }
        }

        K who_ends;
        T depth = 0;
        T cnt_strings_in_subtree = 0;

        T shortest_length_in_subtree = INF;
        T longest_length_in_subtree = 0;
        T min_idx_in_subtree = INF;
        T min_idx_of_shortest_in_subtree = INF;
        T min_idx_of_longest_in_subtree = INF;

        Node(Node* parent, int chr): parent(parent), parent_edge_char(chr) {
            if constexpr(are_strings_distinct) who_ends = -1;
            depth = (parent ? parent->depth : -1) + 1;
        }
    };

    Node* root = new Node(0, -1);
    T cnt_added_strings = 0;
    T cnt_nodes = 1;
    T cnt_different_strings = 0;

    inline bool has(Node* n, int chr) const {
        if constexpr(storage == ARRAY) return n->m[chr];
        else return n->m.count(chr);
    }

    inline bool is_terminal(Node* n) const {
        if constexpr(are_strings_distinct) return n->who_ends != -1;
        else return !n->who_ends.empty();
    }

    Node* get_last_node(const string& s) const {
        Node* n = root;
        for (int chr : s) {
            chr -= FIRST_CHAR;
            if (!has(n, chr)) return n;
            n = n->m[chr];
        }
        return n;
    }

    template<typename T, typename U> bool chmin(T& a, const U& b) {return (T)b < a ? a = b, 1 : 0;}
    template<typename T, typename U> bool chmax(T& a, const U& b) {return (T)b > a ? a = b, 1 : 0;}

public:
    trie() = default;

    void add_string(const string& s) {
        const T idx = cnt_added_strings++;
        auto upd_node = [&](Node* n) {
            ++n->cnt_strings_in_subtree;
            chmin(n->min_idx_in_subtree, idx);
            if (chmin(n->shortest_length_in_subtree, s.size())) {
                n->min_idx_of_shortest_in_subtree = idx;
            }
            if (chmax(n->longest_length_in_subtree, s.size())) {
                n->min_idx_of_longest_in_subtree = idx;
            }
        };
        Node* n = root;
        for (int chr : s) {
            upd_node(n);
            chr -= FIRST_CHAR;
            assert(0 <= chr < ALPHABET);
            if (!has(n, chr)) n->m[chr] = new Node(n, chr), ++cnt_nodes;
            n = n->m[chr];
        }
        upd_node(n);
        cnt_different_strings += !is_terminal(n);
        if constexpr(are_strings_distinct) {
            assert(n->who_ends == -1 && "Strings should be distinct!");
            n->who_ends = idx;
        } else {
            n->who_ends.push_back(idx);         
        }
    }

    T min_idx_of_shortest_with_longest_LCP(const string& s) {
        Node* n = get_last_node(s);
        return n->min_idx_of_shortest_in_subtree;
    }

    T count_containing_as_prefix(const string& s) const {
        Node* n = get_last_node(s);
        return n->depth == s.size() ? n->cnt_strings_in_subtree : 0;
    }

    T LCP(const string& s) const {
        Node* n = get_last_node(s);
        return n->depth;
    }

    bool contains(const string& s) const {
        Node* n = get_last_node(s);
        return n->depth == s.size() && is_terminal(n);
    }

    bool contains_as_prefix(const string& s) const {
        return count_containing_as_prefix(s);
    }
};
