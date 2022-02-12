int inf = 2e9;

struct Node{
    Node *vl, *vr, *nl, *nr;
    int mn=inf;
    Node(Node *a, Node *b, Node *c, Node *d, int n){
        vl = a; vr = b;
        nl = c; nr = d;
        mn = n;
    }
    Node(Node *a, Node *b, Node *c, Node *d){
        vl = a; vr = b;
        nl = c; nr = d;
        if(a) mn = min(mn, a->mn);
        if(b) mn = min(mn, b->mn);
        if(c) mn = min(mn, c->mn);
        if(d) mn = min(mn, d->mn);
    }
};
Node* build(int x1, int y1, int x2, int y2, vec<vec<int>> &m){
    if(x2<x1 || y2<y1) return nullptr;
    if(x1==x2 && y1==y2) return new Node(0, 0, 0, 0, m[x1-1][y1-1]);
    int mx = (x1+x2)>>1, my = (y1+y2)>>1;
    return new Node(build(x1,y1,mx,my,m), build(x1,my+1,mx,y2,m), build(mx+1,y1,x2,my,m), build(mx+1,my+1,x2,y2,m));
}

void set_val(int x, int y, int x1, int y1, int x2, int y2, Node *n, int val){ 
    if(x<x1 || x>x2 || y<y1 || y>y2) return;
    n->mn = min(n->mn, val);
    if(x1==x2 && y1==y2) return;
    int mx = (x1+x2)>>1, my = (y1+y2)>>1;
    set_val(x, y, x1, y1, mx, my, n->vl, val);
    set_val(x, y, x1, my+1, mx, y2, n->vr, val);
    set_val(x, y, mx+1, y1, x2, my, n->nl, val);
    set_val(x, y, mx+1, my+1, x2, y2, n->nr, val);
}

int gm(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, Node *n){
    if(x1>x2 || y1>y2) return inf;
    if(x1==x3 && y1==y3 && x2==x4 && y2==y4) return n->mn;
    int mx = (x3+x4)/2, my = (y3+y4)/2;
    return min({
            gm(x1,y1,min(mx,x2),min(my,y2),x3,y3,mx,my,n->vl),
            gm(x1,max(my+1,y1),min(mx,x2),y2,x3,my+1,mx,y4,n->vr),
            gm(max(mx+1, x1),y1,x2,min(my,y2),mx+1,y3,x4,my,n->nl),
            gm(max(x1,mx+1),max(y1,my+1),x2,y2,mx+1,my+1,x4,y4,n->nr)});
}
