const int G = 200005;
int a, k;
vec<int> l[G];
int is_cp[G] = {0};
int tin[G], fup[G];
int us[G] = {0};

void dfs(int v, int p) {
    static int T = 0;
    us[v] = 1;
    tin[v] = fup[v] = T++;
    int cnt_chd = 0;
    for (int h : l[v]) {
        if (h == p) continue;
        if (!us[h]) {
            cnt_chd++;
            dfs(h, v);
            chmin(fup[v], fup[h]);
            if (fup[h] >= tin[v]) {
                is_cp[v] = 1;
            }
        }
        else {
            chmin(fup[v], tin[h]);
        }
    }
    if (p == -1) {
        is_cp[v] = cnt_chd > 1;
    }
}

int main() {
    fast;
    cin >> a >> k;
    for (int q = 0; q < k; q++) {
        int x, y; cin >> x >> y; x--, y--;
        l[x].pb(y);
        l[y].pb(x);
    }
    for (int q = 0; q < a; q++) {
        if (!us[q]) {
            dfs(q, -1);
        }
    }
    vec<int> ans;
    for (int q = 0; q < a; q++) {
        if (is_cp[q]) {
            ans.pb(q);
        }
    }
    cout << ans.size() << "\n";
    for (int i : ans) cout << i + 1 << " ";
}