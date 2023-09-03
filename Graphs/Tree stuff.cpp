const int G = ?;
int tin[G], tout[G];
int sz[G], dep[G];
int jump[G];
int pr[G];

int is_descendant(int p, int v) {
    return tin[p] <= tin[v] && tout[v] <= tout[p];
}

int kth_ancestor(int v, int k) {
    assert(k <= dep[v]);
    while (k) {
        int u = jump[v], dfd = dep[v] - dep[u];
        if (dfd <= k) v = u, k -= dfd;
        else v = pr[v], k--;
    }
    return v;
}

int LCA(int x, int y) {
    if (dep[x] > dep[y]) swap(x, y);
    y = kth_ancestor(y, dep[y] - dep[x]);
    if (x == y) return x;
    while (pr[x] != pr[y]) {
        int u1 = jump[x], u2 = jump[y];
        if (u1 == u2) x = pr[x], y = pr[y];
        else x = u1, y = u2;
    }
    return pr[x];
}

int get_kth_vertex_on_path(int x, int y, int k, int lc = -1) {
    if (lc == -1) lc = LCA(x, y);
    int dst = dep[x] + dep[y] - dep[lc] * 2;
    if (k > dst) return -1;
    int d1 = dep[x] - dep[lc];
    if (k <= d1) return kth_ancestor(x, k);
    return kth_ancestor(y, dst - k);
}

int wasc[G] = {0};
int find_centroid(int v) {
    int stv = v, p = -1;
    while (true) {
        int mxsz = -1, h = -1;
        for (int to : l[v]) {
            if (to == p || wasc[to]) continue;
            if (sz[to] > mxsz) mxsz = sz[to], h = to;
        }
        if (mxsz <= sz[stv] / 2) return v;
        p = v, v = h;
    }
    return -1;
}

void ifs1(int v, int p = -1) {
    static int T = 0;
    tin[v] = T++;
    pr[v] = p;
    sz[v] = 1;
    dep[v] = p == -1 ? 0 : dep[p] + 1;
    if (p == -1) jump[v] = v;
    else if (dep[pr[v]] + dep[jump[jump[pr[v]]]] == dep[jump[pr[v]]] * 2) jump[v] = jump[jump[pr[v]]];
    else jump[v] = pr[v];
    for (int to : l[v]) {
        if (to == p) continue;
        ifs1(to, v);
        sz[v] += sz[to];
    }
    tout[v] = T;

    //Erasing parent node from adjacency lists
    //if (p != -1) l[v].erase(find(l[v].begin(), l[v].end(), p));

    //Making first child the biggest one
    //for (int& h : l[v]) if (sz[h] > sz[l[v][0]]) swap(h, l[v][0]);
}

int fpth[G];
void ifs2(int v, int fir = -1) {
    static int T = 0;
    if (fir == -1) fir = v;
    tin[v] = T++;
    fpth[v] = fir;
    for (int i = 0; int to : l[v]) ifs2(to, (i++ ? to : fir));
    tout[v] = T;
}
