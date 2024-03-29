//Maintaining MST weight while adding edges online.
//O(Qlog(N) * BIG_CONST)     :-)

#pragma GCC optimize("Ofast")
// #pragma GCC target("avx,avx2,fma")

#include<bits/stdc++.h>

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
#define FILE ifstream in("input.txt"); ofstream out("output.txt");
#define fast cin.tie(0); cout.tie(0); cin.sync_with_stdio(0); cout.sync_with_stdio(0);
using namespace std;
typedef string str;
typedef long long ll;
typedef long double ld;
typedef unsigned int uint;
typedef unsigned long long ull;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

struct Node {
    Node *m[2] = {0, 0};
    Node *p = 0, *pp = 0;
    int x, mx = 0, sz = 0, rev = 0;

    Node() {}

    Node(ll X) {
        x = X;
        sz = 1;
        mx = X;
    }
};

ll gsz(Node *n) {return n ? n->sz : 0;}
int gmx(Node *n) {return n ? n->mx : 0;}

void push(Node *n) {
    if (!n) return;
    if (n->rev) {
        if (n->m[0]) n->m[0]->rev ^= 1;
        if (n->m[1]) n->m[1]->rev ^= 1;
        swap(n->m[0], n->m[1]);
    }
    n->rev = 0;
}

void upd(Node *n) {
    if (!n) return;
    push(n->m[0]);
    push(n->m[1]);
    n->sz = gsz(n->m[0]) + 1 + gsz(n->m[1]);
    n->mx = max({gmx(n->m[0]), n->x, gmx(n->m[1])});
}

void set_path_parent(Node *n, Node *pp) {
    n->pp = pp;
}

void del_path_parent(Node *n) {
    if (!n->pp) return;
    n->pp = 0;
}

void SUPER_PUSH(Node *n) {
    if (n->p) SUPER_PUSH(n->p);
    push(n);
}

int dir(Node *x, Node *par) {
    return par->m[1] == x;
}

void connect(Node *x, Node *par, int d) {
    if (x) x->p = par;
    par->m[d] = x;
    upd(par);
}

void disconnect(Node *par, int d) {
    if (par->m[d]) par->m[d]->p = 0;
    par->m[d] = 0;
    upd(par);
}

void rotate(Node *x) {
    if (!x->p) return;
    Node *p = x->p, *g = p->p;
    int d = dir(x, p);
    if (g) {
        connect(x, g, dir(p, g));
    } else {
        x->p = 0;
    }
    connect(x->m[d ^ 1], p, d);
    connect(p, x, d ^ 1);
}

Node* get_root_of_splay_tree(Node *n) {
    if (n->p) return get_root_of_splay_tree(n->p);
    return n;
}

void splay(Node *n) {
    if (!n) return;
    SUPER_PUSH(n);
    if (!n->p) return;
    Node *rt = get_root_of_splay_tree(n);
    for (; n->p;) {
        if (n->p->p) rotate(dir(n, n->p) == dir(n->p, n->p->p) ? n : n->p);
        rotate(n);
    }
    set_path_parent(n, rt->pp);
    del_path_parent(rt);
}

Node* merge(Node *l, Node *r) {
    if (!l) return r;
    if (!r) return l;
    push(l);
    for (; l->m[1]; ) {
        l = l->m[1];
        push(l);
    }
    splay(l);
    connect(r, l, 1);
    return l;
}

Node* access_by_pos(Node *n, int ps) { //1-index
    for(;;){
        push(n);
        if (ps <= gsz(n->m[0])) n = n->m[0];
        else if (ps > gsz(n->m[0]) + 1) ps = ps - gsz(n->m[0]) - 1, n = n->m[1];
        else break;
    }
    Node *rt = get_root_of_splay_tree(n);
    splay(n);
    splay(rt);
    return n;
}

void expose(Node *n) {
    splay(n);
    if (n->m[1]) {
        set_path_parent(n->m[1], n);
        disconnect(n, 1);
    }
    for (; n->pp;) {
        Node *u = n->pp;
        del_path_parent(n);
        splay(u);
        if (u->m[1]) {
            set_path_parent(u->m[1], u);
            disconnect(u, 1);
        }
        connect(n, u, 1);
        splay(n);
    }
}

Node* get_root_of_LCT(Node *n) {
    expose(n);
    return access_by_pos(n, 1);
}

int is_connected(Node *x, Node *y) {
    return get_root_of_LCT(x) == get_root_of_LCT(y);
}

void link(Node* x, Node* y) {
    if (is_connected(x, y)) return;
    expose(x);
    x->rev ^= 1;
    set_path_parent(x, y);
}

void cut(Node *x) {
    expose(x);
    if (x->m[0]) disconnect(x, 0);
}

void reroot(Node *n) {
    expose(n);
    n->rev ^= 1;
}

const int G = 100005;
int a, Z;
Node* m[G];

ll get_pos_of_max(Node *n) {
    ll ans = 0;
    for (; n; ) {
        push(n);
        if (n->x == gmx(n)) return ans + gsz(n->m[0])+1;
        if (gmx(n->m[0]) == gmx(n)) n = n->m[0];
        else {
            ans += gsz(n->m[0]) + 1;
            n = n->m[1];
        }
    }
    return ans;
}

int main() {
    fast;
    cin >> a >> Z;
    for (int q = 0; q < a; q++) {
        m[q] = new Node(0);
    }
    ll ans = 0;
    for (int I = 0; I < Z; I++) {
        ll w, x, y; cin >> x >> y >> w; x--, y--;
        Node *nw = new Node(w);
        Node *rt = get_root_of_LCT(m[x]);
        if (x == y) {
            cout << ans << "\n";
            continue;
        }
        if (get_root_of_LCT(m[x]) != get_root_of_LCT(m[y])) {
            link(m[y], nw);
            link(m[x], nw);
            reroot(rt);
            ans += w;
        } else {
            Node *rt = get_root_of_LCT(m[x]);
            reroot(m[y]);
            expose(m[x]);
            ll ps = get_pos_of_max(m[x]);
            Node *u = access_by_pos(m[x], ps);
            Node *B = access_by_pos(m[x], ps + 1);
            if (w < u->x) {
                ans -= u->x;
                ans += w;
                cut(u);
                cut(B);
                link(m[x], nw);
                link(m[y], nw);
                reroot(rt);
            }
        }
        cout << ans << "\n";
    }
}
