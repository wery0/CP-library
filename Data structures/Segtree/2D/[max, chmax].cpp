template<typename T, typename C>
class segtree_2d_max_chmax {
    static constexpr C INF = numeric_limits<T>::max();
    static constexpr C DEFAULT_VALUE = 0;
    T minx, miny, maxx, maxy;

    struct Node {
        C iman, sina;
        Node* L = 0;
        Node* R = 0;
        Node() {sina = iman = DEFAULT_VALUE;}

        void seg_chmax(T ly, T ry, C x, T l, T r) {
            if (ry < l || r < ly) return;
            chmax(iman, x);
            if (ly <= l && r <= ry) {
                chmax(sina, x);
                return;
            }
            if (!L) L = new Node(), R = new Node();
            T m = l + (r - l) / 2;
            L->seg_chmax(ly, ry, x, l, m);
            R->seg_chmax(ly, ry, x, m + 1, r);
        }

        C seg_max(T ly, T ry, T l, T r) {
            if (ry < l || r < ly) return DEFAULT_VALUE;
            if (ly <= l && r <= ry) return iman;
            if (!L) return sina;
            T m = l + (r - l) / 2;
            return max({L->seg_max(ly, ry, l, m), R->seg_max(ly, ry, m + 1, r), sina});
        }
    };

    vector<Node*> iman, sina;

    void _rect_chmax(T lx, T rx, T ly, T ry, C x, T l, T r, int v) {
        if (rx < l || r < lx) return;
        iman[v]->seg_chmax(ly, ry, x, miny, maxy);
        if (lx <= l && r <= rx) {
            sina[v]->seg_chmax(ly, ry, x, miny, maxy);
            return;
        }
        T m = l + r >> 1;
        _rect_chmax(lx, rx, ly, ry, x, l, m, v << 1);
        _rect_chmax(lx, rx, ly, ry, x, m + 1, r, v << 1 | 1);
    }

    C _rect_max(T lx, T rx, T ly, T ry, T l, T r, int v) {
        if (rx < l || r < lx) return DEFAULT_VALUE;
        if (lx <= l && r <= rx) return iman[v]->seg_max(ly, ry, miny, maxy);
        T m = l + (r - l) / 2;
        return max({_rect_max(lx, rx, ly, ry, l, m, v << 1), _rect_max(lx, rx, ly, ry, m + 1, r, v << 1 | 1), sina[v]->seg_max(ly, ry, miny, maxy)});
    }

public:
    segtree_2d_max_chmax() = default;
    segtree_2d_max_chmax(T lx, T ly, T rx, T ry): minx(lx), miny(ly), maxx(rx), maxy(ry) {
        assert(lx <= rx && ly <= ry);
        assert(rx - lx < 1e6 && "Memory complexity is O(rx - lx) for x coordinates! Remove this assert if that's fine.");
        int sz = rx - lx + 1; sz = (sz & (sz - 1) ? 2 << __lg(sz) : sz) * 2;
        iman.resize(sz), sina.resize(sz);
        for (auto& n : iman) n = new Node();
        for (auto& n : sina) n = new Node();
    }

    //O(log(rx - lx)log(ry - ly)) time and memory
    void rect_chmax(T lx, T rx, T ly, T ry, C x) {
        assert(lx <= rx && ly <= ry);
        _rect_chmax(lx, rx, ly, ry, x, minx, maxx, 1);
    }

    //O(log(rx - lx)log(ry - ly)) time
    T rect_max(T lx, T rx, T ly, T ry) {
        assert(lx <= rx && ly <= ry);
        return _rect_max(lx, rx, ly, ry, minx, maxx, 1);
    }
};
