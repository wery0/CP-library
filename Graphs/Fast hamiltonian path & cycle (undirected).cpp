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

    vector<vector<int>> used;
    unordered_set<int> caneg;
    void cut(int a, int b) {
        LCT::cut(a, b);
        for (int s = 0; s < 2; ++s) {
            for (int i = 0; i < used[a].size(); ++i) {
                if (used[a][i] == b) {
                    used[a].erase(used[a].begin() + i);
                    break;
                }
            }
            if (used[a].size() == 1) caneg.insert(a);
            swap(a, b);
        }
    }

    void link(int a, int b) {
        LCT::link(a, b);
        for (int s = 0; s < 2; ++s) {
            used[a].push_back(b);
            if (used[a].size() == 2) caneg.erase(a);
            swap(a, b);
        }
    }

    // mx_ch : max number of adding/replacing. Default is (n + 100) * (n + 50)
    // n : number of vertices
    // eg: vector<pair<int, int>> storing all the edges
    // Returns a vector<int> consists of all indices of vertices on the path. Returns empty list if failed to find one.
    // This version finds some random hamiltonian path.
    // If you want path with fixed first and last vertex, you can modify graph by adding 2 vertexes and 2 edges.
    // If you need cycle - enumerate the edge, remove it, and read previous line.
    vector<int> work(const size_t n, vector<pair<int, int>> eg, int64_t mx_ch = -1) {
        if (n == 1) return {0};
        if (mx_ch == -1) mx_ch = 1ll * (n + 100) * (n + 50);    //default
        static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        LCT::init(n);
        used.resize(n + 1);
        caneg.clear();
        for (size_t i = 1; i <= n; ++i) used[i].clear();
        vector<vector<int>> edges(n + 1);
        for (auto& [x, y] : eg) {
            ++x, ++y;
            edges[x].push_back(y);
            edges[y].push_back(x);
        }
        for (int i = 1; i <= n; ++i) caneg.insert(i);
        int tot = 0;
        while (mx_ch >= 0) {
            vector<pair<int, int>> eg;
            for (auto v : caneg) {
                for (auto s : edges[v]) {
                    eg.emplace_back(v, s);
                }
            }
            shuffle(eg.begin(), eg.end(), rng);
            if (eg.size() == 0) break;
            for (auto [x, y] : eg) {
                mx_ch--;
                if (used[x].size() < used[y].size()) swap(x, y);
                if (used[y].size() >= 2 || (rng() & 1) || LCT::fdr(x) == LCT::fdr(y)) continue;
                if (used[x].size() < 2 && used[y].size() < 2) ++tot;
                if (used[x].size() == 2) {
                    int p = used[x][rng() & 1];
                    cut(x, p);
                }
                link(x, y);
            }
            if (tot == n - 1) {
                vector<int> cur;
                for (int i = 1; i <= n; ++i) {
                    if (used[i].size() <= 1) {
                        int pl = i, ls = 0;
                        while (pl) {
                            cur.push_back(pl - 1);
                            int flag = 0;
                            for (auto v : used[pl]) {
                                if (v != ls) {
                                    ls = pl;
                                    pl = v;
                                    flag = 1;
                                    break;
                                }
                            }
                            if (!flag) break;
                        }
                        break;
                    }
                }
                return cur;
            }
        }
        return {};
    }
}
