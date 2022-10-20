//For every rectangle counting number of others, with which we are intersecting by non-zero surface area.
//O(Nlog(N)) offline

#pragma GCC optimize("Ofast")
// #pragma GCC target("avx,avx2,fma")

#include <bits/stdc++.h>

#define F first
#define S second
#define vec vector
#define pb push_back
#define cld complex<ld>
#define pll pair<ll, ll>
#define pdd pair<ld, ld>
#define umap unordered_map
#define uset unordered_set
#define pii pair<int, int>
#define pnn pair<Node*, Node*>
#define all(m) m.begin(), m.end()
#define uid uniform_int_distribution
#define init(m, x) memset(m, x, sizeof(m));
#define pripii(p) cout << "{" << p.F << ", " << p.S << "} "
#define fast cin.tie(0); cout.tie(0); cin.sync_with_stdio(0); cout.sync_with_stdio(0);
using namespace std;
using str = string;
using ll = long long;
using ld = long double;
using uint = unsigned int;
using ull = unsigned long long;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

template<typename T>
bool chmin(T &a, const T &b) { return b < a ? a = b, 1 : 0; }

template<typename T>
bool chmax(T &a, const T &b) { return b > a ? a = b, 1 : 0; }

template<typename T, int G>
struct fenv {
    vec<T> fen = vec<T>(G);
    //T fen[G] = {0};
    //array<T, G> fen{};

    fenv() {}

    fenv(vec<T> &n) {
        for (int q = 1; q <= n.size(); q++) {
            fen[q] += n[q - 1];
            const int nw = q + (q & -q);
            if (nw < G) fen[nw] += fen[q];
        }
    }

    fenv(T val) {
        for (int q = 1; q < G; q++) {
            fen[q] += val;
            const int nw = q + (q & -q);
            if (nw < G) fen[nw] += fen[q];
        }
    }

    void clear() {
        fill(fen.begin(), fen.end(), 0);
    }

    T sum(int p) {
        T o = 0;
        for (; p < G; p += p & -p) {
            o += fen[p];
        }
        return o;
    }

    void pref_add(int p, T x) {
        for (; p; p -= p & -p) {
            fen[p] += x;
        }
    }

    void add_lr(int l, int r, T x) {
        pref_add(r, x);
        pref_add(l - 1, -x);
    }
};

struct rect {
    int x1, y1, x2, y2, n;

    rect() {}

    rect(int a, int b, int c, int d, int e) {
        x1 = a, y1 = b;
        x2 = c, y2 = d;
        n = e;
    }
};


const int G = 1e5 + 5, U = 200020;
int a;
rect m[G];
int ans[G] = {0};

void compress() {
    vec<int> nx, ny;
    for (int q = 0; q < a; q++) {
        nx.pb(m[q].x1);
        nx.pb(m[q].x2);
        ny.pb(m[q].y1);
        ny.pb(m[q].y2);
    }
    sort(all(nx));
    sort(all(ny));
    for (int q = 0; q < a; q++) {
        m[q].x1 = lower_bound(all(nx), m[q].x1) - nx.begin() + 2;
        m[q].x2 = lower_bound(all(nx), m[q].x2) - nx.begin() + 2;

        m[q].y1 = lower_bound(all(ny), m[q].y1) - ny.begin() + 2;
        m[q].y2 = lower_bound(all(ny), m[q].y2) - ny.begin() + 2;
    }
}

struct que {
    int p, n;
    int l, r;
    int sign;

    que() {}

    que(int a, int b, int c, int d, int e) {
        p = a, n = b;
        l = c, r = d;
        sign = e;
    }
};

int all_segs = 0;
fenv<int, 200200> fen_pref = 0;
fenv<int, 200200> fen_suf = 0;

void clear() {
    all_segs = 0;
    fen_pref.clear();
    fen_suf.clear();
}

void add_seg(int l, int r) {
    fen_pref.add_lr(r, U, 1);
    fen_suf.add_lr(1, l, 1);
    all_segs += 1;
}

void rem_seg(int l, int r) {
    fen_pref.add_lr(r, U, -1);
    fen_suf.add_lr(1, l, -1);
    all_segs -= 1;
}

int cnt_intersections(int l, int r) {
    return all_segs - fen_pref.sum(l) - fen_suf.sum(r);
}

vec<pii > del_seg[U];

int main() {
    fast;
    cin >> a;
    for (int q = 0; q < a; q++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        m[q] = rect(x1, y1, x2, y2, q);
    }
    compress();
    sort(m, m + a, [](rect &r1, rect &r2) {
        return r1.x1 < r2.x1;
    });
    for (int x = 0, l = 0; l < a; x++) {
        for (; del_seg[x].size();) {
            pii p = del_seg[x].back();
            del_seg[x].pop_back();
            rem_seg(p.F, p.S);
        }
        if (m[l].x1 != x) continue;
        int r = l;
        for (; r + 1 < a && m[r + 1].x1 == x;) {
            r++;
        }
        for (int w = l; w <= r; w++) {
            ans[m[w].n] += cnt_intersections(m[w].y1, m[w].y2);
        }
        for (int w = l; w <= r; w++) {
            add_seg(m[w].y1, m[w].y2);
            del_seg[m[w].x2].pb({m[w].y1, m[w].y2});
        }
        l = r + 1;
    }
    vec<que> n;
    for (int q = 0; q < a; q++) {
        n.pb(que(m[q].x1, m[q].n, m[q].y1, m[q].y2, 1));
        n.pb(que(m[q].x2, m[q].n, m[q].y1, m[q].y2, -1));
    }
    sort(all(n), [](que &q1, que &q2) {
        return q1.p > q2.p;
    });
    reverse(m, m + a);
    clear();
    for (int x = U - 1, l = 0, ql = 0; l < a || ql < n.size(); x--) {
        int r = l - 1;
        for (; r + 1 < a && m[r + 1].x1 == x;) {
            r++;
        }
        for (int w = l; w <= r; w++) {
            add_seg(m[w].y1, m[w].y2);
        }
        for (; ql < n.size() && n[ql].p == x; ql++) {
            int res = cnt_intersections(n[ql].l, n[ql].r);
            ans[n[ql].n] += res * n[ql].sign;
        }
        l = r + 1;
    }
    for (int q = 0; q < a; q++) {
        cout << ans[q] - 1 << " ";
    }
}
