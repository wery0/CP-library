int tin[G], tout[G];
int sz[G], dep[G];
int pr[G];

int is_descendant(int p, int v) {
    return tin[p] <= tin[v] && tout[v] <= tout[p];
}

void ifs1(int v, int p = -1) {
    pr[v] = p;
    sz[v] = 1;
    dep[v] = p == -1 ? 0 : dep[p] + 1;
    for (int to : l[v]) {
        if (to == p) continue;
        ifs1(to, v);
        sz[v] += sz[to];
    }
    if (p != -1) l[v].erase(find(all(l[v]), p));
    for (int &h : l[v]) if (sz[h] > sz[l[v][0]]) swap(h, l[v][0]);
}

int fpth[G];
void ifs2(int v, int fir = -1) {
    if (fir == -1) fir = v;
    static int T = 0;
    tin[v] = T++;
    fpth[v] = fir;
    for (int q = 0; q < l[v].size(); q++) {
        const int to = l[v][q];
        ifs2(to, (q ? to : fir));
    }
    tout[v] = T;
}

auto path_query(int x, int y) {
    auto query = [&](int fr, int to) {
        //assert(fpth[fr] == fpth[to]);
        if (tin[fr] > tin[to]) swap(fr, to);
        return seg_query(tin[fr], tin[to]);
    };
    array<int, 2> arr = {x, y};
    for (int &v : arr) {
        int u = x + y - v;
        for (; !is_descendant(fpth[v], u);) {
            query(fpth[v], v);
            v = pr[fpth[v]];
        }
    }
    query(arr[0], arr[1]);
    return ?;
}

auto path_query_inorder(int x, int y) {
    auto query = [&](int fr, int to) {
        //assert(fpth[fr] == fpth[to]);
        if (tin[fr] > tin[to]) {
            swap(fr, to);
            //order of vertices may affect the answer. Process this case here, if need.
        }
        return seg_query(tin[fr], tin[to]);
    };
    vec<pii> store;
    for (; !is_descendant(fpth[x], y);) {
        store.pb({x, fpth[x]});
        x = pr[fpth[x]];
    }
    int sz = store.size();
    for (; fpth[x] != fpth[y];) {
        store.pb({fpth[y], y});
        y = pr[fpth[y]];
    }
    reverse(store.begin() + sz, store.end());
    store.insert(store.begin() + sz, {x, y});
    for (pii p : store) {
        query(p.F, p.S);
    }
    return ?;
}

auto subtree_query(int v) {
    return seg_query(tin[v], tout[v] - 1);
}

ifs1(0), ifs2(0);