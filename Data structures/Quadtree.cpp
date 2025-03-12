template<typename T>
class quadtree {
    struct Node {
        Node* m[4] = {0};
        int cnt = 0;

        Node() = default;
    };
    Node* root = new Node();
    T lx_, ly_, rx_, ry_;

    bool is_inside(T x, T l, T r) const {return l <= x && x <= r;}

public:
    quadtree() = default;
    quadtree(T c): lx_(-c), ly_(-c), rx_(c), ry_(c) {}
    quadtree(T lx, T ly, T rx, T ry): lx_(lx), ly_(ly), rx_(rx), ry_(ry) {
        assert(lx <= rx && ly <= ry);
    }

    bool insert(T x, T y) {
        static vector<Node*> store;
        Node* n = root;
        T lx = lx_, ly = ly_, rx = rx_, ry = ry_;
        bool was = 0;
        for (;;) {
            store.push_back(n);
            if (lx == rx && ly == ry) {
                was = n->cnt;
                break;
            }
            T mx = lx + (rx - lx) / 2, my = ly + (ry - ly) / 2;
            int part = is_inside(x, mx + 1, rx) + is_inside(y, my + 1, ry) * 2;
            if (!n->m[part]) n->m[part] = new Node();
            (part & 1 ? lx = mx + 1 : rx = mx);
            (part & 2 ? ly = my + 1 : ry = my);
            n = n->m[part];
        }
        if (!was) {
            for (Node* u : store) {
                ++u->cnt;
            }
        }
        store.clear();
        return !was;
    }

    bool erase(T x, T y) {
        static vector<Node*> store;
        Node* n = root;
        T lx = lx_, ly = ly_, rx = rx_, ry = ry_;
        bool was = 0;
        for (; n;) {
            store.push_back(n);
            if (lx == rx && ly == ry) {
                was = n->cnt;
                break;
            }
            T mx = lx + (rx - lx) / 2, my = ly + (ry - ly) / 2;
            int part = is_inside(x, mx + 1, rx) + is_inside(y, my + 1, ry) * 2;
            (part & 1 ? lx = mx + 1 : rx = mx);
            (part & 2 ? ly = my + 1 : ry = my);
            n = n->m[part];
        }
        if (was) {
            for (Node* u : store) {
                --u->cnt;
            }
        }
        store.clear();
        return was;
    }
};
