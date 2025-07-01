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
    if (p != -1) l[v].erase(find(l[v].begin(), l[v].end(), p));
    for (int& h : l[v]) if (sz[h] > sz[l[v][0]]) swap(h, l[v][0]);
}

int fpth[G];
void ifs2(int v, int fir = -1) {
    if (fir == -1) fir = v;
    static int T = 0;
    tin[v] = T++;
    fpth[v] = fir;
    for (int i = 0; i < l[v].size(); ++i) {
        const int to = l[v][i];
        ifs2(to, (i ? to : fir));
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
    for (int& v : arr) {
        int u = x + y - v;
        while (!is_descendant(fpth[v], u)) {
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
    vector<pair<int, int>> store;
    while (!is_descendant(fpth[x], y)) {
        store.emplace_back(x, fpth[x]);
        x = pr[fpth[x]];
    }
    int sz = store.size();
    while (fpth[x] != fpth[y]) {
        store.emplace_back(fpth[y], y);
        y = pr[fpth[y]];
    }
    reverse(store.begin() + sz, store.end());
    store.insert(store.begin() + sz, {x, y});
    for (auto [x, y] : store) query(x, y);
    return ?;
}

auto subtree_query(int v) {
    return seg_query(tin[v], tout[v] - 1);
}

ifs1(0), ifs2(0);
