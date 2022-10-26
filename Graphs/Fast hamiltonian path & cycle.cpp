namespace hamil {
    namespace LCT {

        vec<vec<int>> ch;
        vec<int> fa, rev;

        void init(int n) {
            ch.resize(n + 1);
            fa.resize(n + 1);
            rev.resize(n + 1);
            for (int i = 0; i <= n; i++)
                ch[i].resize(2),
                ch[i][0] = ch[i][1] = fa[i] = rev[i] = 0;
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

    vec<int> out, in;
    mt19937 x(timeStamp().time_since_epoch().count());
    vec<int> work(int n, vector<pii> eg, ll mx_ch = -1) {
        // mx_ch : max number of adding/replacing. Default is (n + 100) * (n + 50)
        // n : number of vertices
        // eg: vector<pair<int, int>> storing all the edges
        // Returns a vector<int> consists of all indices of vertices on the path. Returns empty list if failed to find one.
        // This version finds some random hamiltonian path.
        // If you want path with fixed first and last vertex, you can modify graph by adding 2 vertexes and 2 edges.
        // If you need cycle - enumerate the edge, remove it, and check previous line.
        out.resize(n + 1), in.resize(n + 1);
        LCT::init(n);
        for (int i = 0; i <= n; i++) in[i] = out[i] = 0;
        if (mx_ch == -1) mx_ch = 1ll * (n + 100) * (n + 50); //default
        vec<vec<int>> from(n + 1), to(n + 1);
        for (auto &p : eg) ++p.F, ++p.S;
        for (auto v : eg) {
            from[v.F].pb(v.S);
            to[v.S].pb(v.F);
        }
        uset<int> canin, canout;
        for (int i = 1; i <= n; i++) {
            canin.insert(i);
            canout.insert(i);
        }
        int tot = 0;
        while (mx_ch >= 0) {
            vec<pii> eg;
            for (auto v : canout) for (auto s : from[v]) if (in[s]) eg.pb({v, s});
            for (auto v : canin) for (auto s : to[v]) eg.pb({s, v});
            shuffle(all(eg), x);
            if (eg.empty()) break;
            for (auto v : eg) {
                mx_ch--;
                if (in[v.S] && out[v.F]) continue;
                if (LCT::fdr(v.F) == LCT::fdr(v.S)) continue;
                if ((in[v.S] || out[v.F]) && (x() & 1)) continue;
                tot += !in[v.S] && !out[v.F];
                if (in[v.S]) {
                    LCT::cut(in[v.S], v.S);
                    canin.insert(v.S);
                    canout.insert(in[v.S]);
                    out[in[v.S]] = 0;
                    in[v.S] = 0;
                }
                if (out[v.F]) {
                    LCT::cut(v.F, out[v.F]);
                    canin.insert(out[v.F]);
                    canout.insert(v.F);
                    in[out[v.F]] = 0;
                    out[v.F] = 0;
                }
                LCT::link(v.F, v.S);
                canin.erase(v.S);
                canout.erase(v.F);
                in[v.S] = v.F;
                out[v.F] = v.S;
            }
            if (tot == n - 1) {
                vec<int> cur;
                for (int i = 1; i <= n; i++) {
                    if (!in[i]) {
                        int pl = i;
                        while (pl) {
                            cur.pb(pl);
                            pl = out[pl];
                        }
                        break;
                    }
                }
                for (auto &v : cur) --v;
                return cur;
            }
        }
        //failed to find a path
        return vec<int>();
    }
}