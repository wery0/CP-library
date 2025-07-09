namespace hamil {
    namespace LCT {

        vector<array<int, 2>> ch;
        vector<int> fa, rev;

        void init(int n) {
            ch = vector<array<int, 2>>(n + 1, {0, 0});
            fa = vector<int>(n + 1);
            rev = vector<int>(n + 1);
        }

        bool isr(int a) {return !(ch[fa[a]][0] == a || ch[fa[a]][1] == a);}

        void pushdown(int a) {
            if (rev[a]) {
                rev[ch[a][0]] ^= 1, rev[ch[a][1]] ^= 1;
                swap(ch[a][0], ch[a][1]);
                rev[a] = 0;
            }
        }

        void push(int a) {
            if (!isr(a)) push(fa[a]);
            pushdown(a);
        }

        void rotate(int a) {
            int f = fa[a], gf = fa[f];
            int tp = ch[f][1] == a;
            int son = ch[a][tp ^ 1];
            if (!isr(f)) ch[gf][ch[gf][1] == f] = a;
            fa[a] = gf;
            ch[f][tp] = son;
            if (son) fa[son] = f;
            ch[a][tp ^ 1] = f, fa[f] = a;
        }

        void splay(int a) {
            push(a);
            while (!isr(a)) {
                int f = fa[a], gf = fa[f];
                if (isr(f)) rotate(a);
                else {
                    int t1 = ch[gf][1] == f, t2 = ch[f][1] == a;
                    if (t1 == t2) rotate(f), rotate(a);
                    else rotate(a), rotate(a);
                }
            }
        }

        void access(int a) {
            int pr = a;
            splay(a);
            ch[a][1] = 0;
            while (1) {
                if (!fa[a]) break;
                int u = fa[a];
                splay(u);
                ch[u][1] = a;
                a = u;
            }
            splay(pr);
        }

        void makeroot(int a) {
            access(a);
            rev[a] ^= 1;
        }

        void link(int a, int b) {
            makeroot(a);
            fa[a] = b;
        }

        void cut(int a, int b) {
            makeroot(a);
            access(b);
            fa[a] = 0, ch[b][0] = 0;
        }

        int fdr(int a) {
            access(a);
            while (1) {
                pushdown(a);
                if (ch[a][0]) a = ch[a][0];
                else {
                    splay(a);
                    return a;
                }
            }
        }
    }

    // Returns a vector of all indices of vertices on the path if it was found, otherwise returns {}.
    // If you want path with fixed first and last vertex, you can modify graph by adding 2 vertexes and 2 edges.
    // If you need cycle - enumerate the edge, remove it, and read previous line.
    vector<int> try_find_hamiltonian_path(const size_t n, vector<pair<int, int>> edges, int64_t mx_ch = -1) {
        if (n == 1) return {0};
        if (mx_ch == -1) mx_ch = (n + 100) * (n + 50);
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        LCT::init(n);
        vector<int> out(n + 1), in(n + 1);
        vector<vector<int>> from(n + 1), to(n + 1);
        for (auto [x, y] : edges) {
            assert(0 <= x && x < n);
            assert(0 <= y && y < n);
            ++x, ++y;
            from[x].push_back(y);
            to[y].push_back(x);
        }
        unordered_set<int> canin, canout;
        for (int i = 1; i <= n; ++i) {
            canin.insert(i);
            canout.insert(i);
        }
        for (int tot = 0; mx_ch > 0; ) {
            edges.clear();
            for (auto v : canout) for (auto s : from[v]) if (in[s]) edges.emplace_back(v, s);
            for (auto v : canin) for (auto s : to[v]) edges.emplace_back(s, v);
            shuffle(edges.begin(), edges.end(), rng);
            if (edges.empty()) break;
            for (auto [x, y] : edges) {
                --mx_ch;
                if (in[y] && out[x]) continue;
                if (LCT::fdr(x) == LCT::fdr(y)) continue;
                if ((in[y] || out[x]) && (rng() & 1)) continue;
                tot += !in[y] && !out[x];
                if (in[y]) {
                    LCT::cut(in[y], y);
                    canin.insert(y);
                    canout.insert(in[y]);
                    out[in[y]] = 0;
                    in[y] = 0;
                }
                if (out[x]) {
                    LCT::cut(x, out[x]);
                    canin.insert(out[x]);
                    canout.insert(x);
                    in[out[x]] = 0;
                    out[x] = 0;
                }
                LCT::link(x, y);
                canin.erase(y);
                canout.erase(x);
                in[y] = x;
                out[x] = y;
            }
            if (tot == n - 1) {
                vector<int> cur;
                for (int i = 1; i <= n; ++i) {
                    if (!in[i]) {
                        int pl = i;
                        while (pl) {
                            cur.push_back(pl);
                            pl = out[pl];
                        }
                        break;
                    }
                }
                for (auto& v : cur) --v;
                return cur;
            }
        }
        return {};
    }
}
