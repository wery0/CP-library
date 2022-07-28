template<typename T, const int BIT_LEN>
struct set_xor_min {
    struct Node {
        Node *m[2];
        int sm = 0;

        Node() {for (auto &i : m) i = nullptr;}
    };

    inline int gsm(Node *n) {return n ? n->sm : 0;}

    Node* root = new Node();

    int last_insert_res;
    void insert(Node *&n, T c, int bit_num) {
        if (!n) n = new Node();
        if (bit_num == -1) {
            last_insert_res = n->sm == 0;
            n->sm = 1;
            return;
        }
        insert(n->m[c >> bit_num & 1], c, bit_num - 1);
        n->sm += last_insert_res;
    }
    void insert(T c) {insert(root, c, BIT_LEN - 1);}

    int last_erase_res;
    void erase(Node *n, T c, int bit_num) {
        if (!n) {
            last_erase_res = 0;
            return;
        }
        if (bit_num == -1) {
            last_erase_res = n->sm == 1;
            n->sm = 0;
            return;
        }
        erase(n->m[c >> bit_num & 1], c, bit_num - 1);
        n->sm -= last_erase_res;
    }
    void erase(T c) {erase(root, c, BIT_LEN - 1);}

    T xor_min(T c) {
        assert(root->sm);
        Node *n = root;
        T best = 0;
        for (int bit_num = BIT_LEN - 1; bit_num >= 0; --bit_num) {
            int bt = c >> bit_num & 1;
            int dir = gsm(n->m[bt]) ? bt : bt ^ 1;
            n = n->m[dir];
            best |= (T)dir << bit_num;
        }
        return best ^ c;
    }

    T xor_max(T c) {
        assert(root->sm);
        Node *n = root;
        T best = 0;
        for (int bit_num = BIT_LEN - 1; bit_num >= 0; --bit_num) {
            int bt = ~c >> bit_num & 1;
            int dir = gsm(n->m[bt]) ? bt : bt ^ 1;
            n = n->m[dir];
            best |= (T)dir << bit_num;
        }
        return best ^ c;
    }
};