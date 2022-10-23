ll inf = 1e15+7;
const ll FORBIDDEN = inf;

struct Node{
    Node *m[2] = {0,0};
    Node *p = 0, *pp = 0;
    ll num = -1;
    ll x=-1, sz = 0, rev = 0;
    ll mn = inf, mx = -inf, sm = 0;
    ll ps_set = FORBIDDEN, ps_add = 0;

    Node(){}

    Node(ll X, ll N){
        x = X;
        num = N;
        mn = mx = sm = X;
        sz = 1;
    }
};

void print(Node *n){
    if(!n) return;
    print(n->m[0]);
    cout<<n->x<<" ";
    print(n->m[1]);
}

ll gsz(Node *n){return n ? n->sz : 0;}
ll gmn(Node *n){return n ? n->mn : inf;}
ll gmx(Node *n){return n ? n->mx : -inf;}
ll gsm(Node *n){return n ? n->sm : 0;}

void incr_ps_add(Node *n, ll c){
    if(!n) return;
    if(n->ps_set != FORBIDDEN) n->ps_set += c;
    else n->ps_add += c;
}

void set_ps_set(Node *n, ll c){
    if(!n) return;
    n->ps_set = c;
    n->ps_add = 0;
}

void push(Node *n){
    if(!n) return;
    if(n->rev) {
        if (n->m[0]) n->m[0]->rev ^= 1;
        if (n->m[1]) n->m[1]->rev ^= 1;
        swap(n->m[0], n->m[1]);
    }
    if(n->ps_add){
        ll c = n->ps_add;
        incr_ps_add(n->m[0], c);
        incr_ps_add(n->m[1], c);
        n->x += c;
        n->mx += c;
        n->mn += c;
        n->sm += gsz(n) * c;
    }
    if(n->ps_set!=FORBIDDEN){
        ll c = n->ps_set;
        set_ps_set(n->m[0], c);
        set_ps_set(n->m[1], c);
        n->x = c;
        n->mn = c;
        n->mx = c;
        n->sm = gsz(n) * c;
    }
    n->rev = 0;
    n->ps_add = 0;
    n->ps_set = FORBIDDEN;
}

void upd(Node *n){
    if(!n) return;
    push(n->m[0]);
    push(n->m[1]);
    n->sz = gsz(n->m[0]) + 1 + gsz(n->m[1]);
    n->sm = gsm(n->m[0]) + n->x + gsm(n->m[1]);
    n->mn = min({gmn(n->m[0]), n->x, gmn(n->m[1])});
    n->mx = max({gmx(n->m[0]), n->x, gmx(n->m[1])});
}

void set_path_parent(Node *n, Node *pp){
    assert(!n->pp);
    n->pp = pp;
}

void del_path_parent(Node *n){
    if(!n->pp) return;
    n->pp = 0;
}

void SUPER_PUSH(Node *n){
    if(n->p) SUPER_PUSH(n->p);
    push(n);
}

int dir(Node *x, Node *par){
    return par->m[1]==x;
}

void connect(Node *x, Node *par, int d){
    if(x) x->p = par;
    par->m[d] = x;
    upd(par);
}

void disconnect(Node *par, int d){
    if(par->m[d]) par->m[d]->p = 0;
    par->m[d] = 0;
    upd(par);
}

void rotate(Node *x){
    if(!x->p) return;
    Node *p = x->p, *g = p->p;
    int d = dir(x, p);
    if(g) connect(x, g, dir(p, g));
    else x->p = 0;
    connect(x->m[d^1], p, d);
    connect(p, x, d ^ 1);
}

Node* get_root_of_splay_tree(Node *n){
    if(n->p) return get_root_of_splay_tree(n->p);
    return n;
}

void splay(Node *n){
    if(!n) return;
    SUPER_PUSH(n);
    if(!n->p) return;
    Node *rt = get_root_of_splay_tree(n);
    for(; n->p;){
        if(n->p->p) rotate(dir(n, n->p)==dir(n->p, n->p->p) ? n->p : n);
        rotate(n);
    }
    set_path_parent(n, rt->pp);
    del_path_parent(rt);
}

Node* merge(Node *l, Node *r){
    if(!l) return r;
    if(!r) return l;
    push(l);
    for(; l->m[1]; ){
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

pnn splitSz(Node *n, int k){
    if(!k) return {0,n};
    n = access_by_pos(n, k);
    Node *R = n->m[1];
    if(R) disconnect(n, 1);
    return {n, R};
}

Node* rem_by_pos(Node *n, int pos){
    pnn p1 = splitSz(n, pos-1), p2 = splitSz(p1.S, 1);
    return merge(p1.F, p2.S);
}

int get_val(Node *&n, int pos){
    n = access_by_pos(n, pos);
    return n->x;
}

void seg_rev(Node *&n, int l, int r){
    pnn p = splitSz(n, l-1), p1 = splitSz(p.S, r-l+1);
    p1.F->rev ^= 1;
    n = merge(p.F, merge(p1.F, p1.S));
}

void expose(Node *n){
    splay(n);
    if(n->m[1]){
        set_path_parent(n->m[1], n);
        disconnect(n, 1);
    }
    for(; n->pp;){
        Node *u = n->pp;
        //n->pp = 0;
        del_path_parent(n);
        splay(u);
        if(u->m[1]){
            set_path_parent(u->m[1], u);
            disconnect(u, 1);
        }
        connect(n, u, 1);
        splay(n);
    }
}

Node* get_root_of_LCT(Node *n){
    expose(n);
    return access_by_pos(n, 1);
}

int is_connected(Node *x, Node *y){
    return get_root_of_LCT(x)==get_root_of_LCT(y);
}

int depth(Node *x){
    expose(x);
    return gsz(x);
}

void reroot(Node *n){
    expose(n);
    n->rev ^= 1;
}

void link(Node* n, Node* p){
    if(is_connected(n, p)) return;
    reroot(n);
    set_path_parent(n, p);
}

void cut(Node *x, Node *y){
    if(depth(x)<depth(y)) swap(x, y);
    expose(y);
    splay(x);
    del_path_parent(x);
}


void cut(Node *x){
    expose(x);
    if(x->m[0]) disconnect(x, 0);
}

Node* lca(Node *x, Node *y){
    if(x==y) return x;
    if(depth(x)<depth(y)) swap(x, y);
    expose(x);
    expose(y);
    return x->pp;
}

const int G = 100005;
int a,Z,root = -1;
Node* m[G];

int main() {
    fast;
    cin>>a>>Z;
    for(int q=0; q<a; q++){
        int x; cin>>x;
        m[q] = new Node(x, q);
    }
    for(int q=0; q<a-1; q++){
        int x,y; cin>>x>>y; x--, y--;
        link(m[x], m[y]);
    }
    cin>>root; root--;
    reroot(m[root]);
    for(int I=0; I<Z; I++){
        int type; cin>>type;
        if(type==0){
            cin>>root; root--;
            reroot(m[root]);
        }
        if(type==1){
            int x,y,z; cin>>x>>y>>z; x--, y--;
            reroot(m[x]);
            expose(m[y]);
            set_ps_set(m[y], z);
            reroot(m[root]);
        }
        if(type==2){
            int x,y,z; cin>>x>>y>>z; x--, y--;
            reroot(m[x]);
            expose(m[y]);
            incr_ps_add(m[y], z);
            reroot(m[root]);
        }
        if(3<=type && type<=5){
            int x,y; cin>>x>>y; x--, y--;
            reroot(m[y]);
            expose(m[x]);
            ll o = type==3 ? m[x]->mn : type==4 ? m[x]->mx : m[x]->sm;
            cout<<o<<"\n";
            reroot(m[root]);
        }
        if(type==6){
            int x,y; cin>>x>>y; x--, y--;
            if(lca(m[x], m[y])==m[x]) continue;
            cut(m[x]);
            link(m[x], m[y]);
        }
        if(type==7){
            int x,y; cin>>x>>y; x--, y--;
            Node *u = lca(m[x], m[y]);
            cout<<u->num+1<<"\n";
        }
    }
}