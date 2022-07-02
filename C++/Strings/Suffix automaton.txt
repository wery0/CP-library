ll T = 1;

struct Node {
    Node *m[26] = {0};
    Node *p = 0, *sl = 0;
    ll d = 0;
    ll cnt = 0;
    ll rg = -1;
    ll us = 0;

    Node() {}

    Node(Node *P) {
        p = P;
        d = p->d + 1;
    }
};

Node *lst, *root;
ll ans = 0;
ll cur_rg = 0;

void add_char(char c) {
    c -= 'a'; assert(0 <= c && c < 26);
    Node *y = new Node(lst);
    y->rg = cur_rg;
    y->sl = root;
    for (Node *u = lst; u; u = u->sl) {
        if (!u->m[c]) {
            u->m[c] = y;
            y->cnt += u->cnt;
            ans += u->cnt;
            continue;
        }
        Node *j = u->m[c];
        if (j->p == u) {
            y->sl = j;
            break;
        }
        Node *nw = new Node(u);
        nw->rg = cur_rg;
        nw->sl = j->sl;
        j->sl = nw;
        y->sl = nw;
        memcpy(nw->m, j->m, sizeof(nw->m));
        ll e = 0;
        for (Node *t = u; t && t->m[c] == j; t = t->sl) {
            t->m[c] = nw;
            e += t->cnt;
        }
        nw->cnt = e;
        j->cnt -= e;
        break;
    }
    lst = y->p->m[c];
}

ll a;
str t;
vec<ll> ps[26];

int cnt_lr(int c, int l, int r) {
    assert(0 <= c && c < 26);
    return upper_bound(all(ps[c]), r) - lower_bound(all(ps[c]), l);
}

ll o = 0;
void dfs(Node *n) {
    if (!n || n->us == T) return;
    n->us = T;
    for (int q = 0; q < 26; q++) {
        dfs(n->m[q]);
    }
    if (n->rg == -1) return;
    ll l1 = n->rg - n->d + 1, r = n->rg;
    ll l2 = r - n->sl->d;
    if (l1 < l2) {
        for (int q = 0; q < 26; q++) {
            o += n->m[q] ? cnt_lr(q, l1, l2 - 1) : 0;
        }
    }
    o += n->sl->m[t[l2] - 'a'] ? 1 : 0;
}

int main() {
    fast;
    cin >> t;
    a = t.size();
    lst = new Node();
    lst->cnt = 1;
    root = lst;
    for (int q = 0; q < a; q++) {
        ps[t[q] - 'a'].pb(q);
        add_char(t[q]);
        cur_rg++;
    }
    dfs(root);
    cout << o;
}
