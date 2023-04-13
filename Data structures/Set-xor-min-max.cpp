//Works as std::set, but also supports xor_min and xor_max operations.
//Xor_min(max)(C) finds min(max) xor of C and element from set.
//O(BIT_LENGTH) time per any operation.
//Works with numbers in range [0; 2^BIT_LENGTH)
template<typename T, const int BIT_LENGTH>
class set_xor_min_max {
    struct Node {
        Node* m[2];
        int sm = 0;

        Node() {for (auto& i : m) i = 0;}
    };

    Node* root = new Node();

    int gsm(Node* n) const {return n ? n->sm : 0;}

    void insert(Node*& n, T x, int bit_num) {
        static int last_insert_res;
        if (!n) n = new Node();
        if (bit_num == -1) {
            last_insert_res = n->sm == 0;
            n->sm = 1;
            return;
        }
        insert(n->m[x >> bit_num & 1], x, bit_num - 1);
        n->sm += last_insert_res;
    }

    void erase(Node* n, T x, int bit_num) {
        static int last_erase_res;
        if (!n) {
            last_erase_res = 0;
            return;
        }
        if (bit_num == -1) {
            last_erase_res = n->sm == 1;
            n->sm = 0;
            return;
        }
        erase(n->m[x >> bit_num & 1], x, bit_num - 1);
        n->sm -= last_erase_res;
    }

    T xor_query(T x, const int is_max) const {
        assert(!empty());
        if (is_max) x = ~x;
        Node* n = root;
        T best = 0;
        for (int bit_num = BIT_LENGTH - 1; bit_num >= 0; --bit_num) {
            int bt = x >> bit_num & 1;
            int dir = gsm(n->m[bt]) ? bt : bt ^ 1;
            n = n->m[dir];
            best |= (T)dir << bit_num;
        }
        return best ^ x;
    }

    int count(Node* n, T x) const {
        for (int bit_num = BIT_LENGTH - 1; bit_num >= 0; --bit_num) {
            int bt = x >> bit_num & 1;
            if (!gsm(n->m[bt])) return 0;
            n = n->m[bt];
        }
        return gsm(n);
    }

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->m[0]);
        destroy(n->m[1]);
        delete n;
    }

public:
    set_xor_min_max() = default;
    ~set_xor_min_max() {destroy(root);}

    void insert(T x) {insert(root, x, BIT_LENGTH - 1);}
    void erase(T x) {erase(root, x, BIT_LENGTH - 1);}

    T xor_min(T x) const {return xor_query(x, 0);}
    T xor_max(T x) const {return xor_query(x, 1);}

    size_t size() const {return root->sm;}
    bool empty() const {return root->sm == 0;}

    int count(T x) const {return count(root, x);}
    void clear() {destroy(root); root = new Node();}
};