struct segtree {

    struct Node {
        ll sm = 0;
        ll mn1, mx1, mn2, mx2;
        ll cmn1 = 0, cmx1 = 0;
        ll ps_add = 0;
        ll dgc = 0;           //without mn1 and mx1

        Node() {}

        Node(ll x) {
            sm = x;
            mn1 = mx1 = x;
            cmn1 = cmx1 = 1;
        }
    };

    const ll inf = 1LL << 60;
    const ll NO = 4e18 + 228337;     //change, if need
    vec<Node> m;
    uint a, U;

    segtree() = default;

    segtree(vec<ll> &n) {
        a = n.size();
        U = geq_pow2(a);
        m.resize(U * 2);
        for (uint q = 0; q < a; q++) {
            m[U + q] = Node(n[q]);
        }
        for (uint q = U; --q;) {
            upd(q);
        }
    }

    inline constexpr int gsz(int v) {
        return 1 << (__lg(U) - __lg(v));
    }

    ll gcd(ll x, ll y) {
        if (x > y) swap(x, y);
        for (; x;) {
            y %= x;
            if (!y) break;
            x %= y;
        }
        return x + y;
    }

    ll calcmn2(Node &l, Node &r) {
        ll mn1 = min(l.mn1, r.mn1);
        ll mn2 = inf;
        if (l.mn1 != mn1) mn2 = min(mn2 , l.mn1);
        if (r.mn1 != mn1) mn2 = min(mn2 , r.mn1);
        if (l.mn1 != l.mx1) mn2 = min(mn2 , l.mn2);
        if (r.mn1 != r.mx1) mn2 = min(mn2 , r.mn2);
        return mn2;
    }

    ll calcmx2(Node &l, Node &r) {
        ll mx1 = max(l.mx1, r.mx1);
        ll mx2 = -inf;
        if (l.mx1 != mx1) mx2 = max(mx2 , l.mx1);
        if (r.mx1 != mx1) mx2 = max(mx2 , r.mx1);
        if (l.mn1 != l.mx1) mx2 = max(mx2 , l.mx2);
        if (r.mn1 != r.mx1) mx2 = max(mx2 , r.mx2);
        return mx2;
    }

    ll chk(ll x, ll a, ll b) {
        return x != a && x != b;
    }

    ll get_not_extremum(Node &n, ll mn, ll mx) {
        if (n.mx1 == n.mn1) return chk(n.mx1, mn, mx) ? n.mx1 : NO;
        if (n.mx2 == n.mn1) return chk(n.mx1, mn, mx) ? n.mx1 : chk(n.mx2, mn, mx) ? n.mx2 : NO;
        if (chk(n.mx1, mn, mx)) return n.mx1;
        if (chk(n.mx2, mn, mx)) return n.mx2;
        return n.mn1;
    }

    void upd(int v) {
        if (v >= U) return;
        Node &n = m[v], &l = m[v << 1], &r = m[v << 1 | 1];
        n.sm = l.sm + r.sm;
        n.mn1 = min(l.mn1, r.mn1);
        n.mx1 = max(l.mx1, r.mx1);
        n.cmn1 = (l.mn1 == n.mn1 ? l.cmn1 : 0) + (r.mn1 == n.mn1 ? r.cmn1 : 0);
        n.cmx1 = (l.mx1 == n.mx1 ? l.cmx1 : 0) + (r.mx1 == n.mx1 ? r.cmx1 : 0);
        n.mn2 = calcmn2(l, r);
        n.mx2 = calcmx2(l, r);
        n.dgc = gcd(l.dgc, r.dgc);
        ll lf = get_not_extremum(l, l.mn1, l.mx1);
        ll rg = get_not_extremum(r, r.mn1, r.mx1);
        if (lf != NO && rg != NO) n.dgc = gcd(n.dgc, abs(lf - rg));
        ll any = lf == NO ? rg == NO ? -1 : rg : lf;
        for (ll val : {l.mn1, l.mx1, r.mn1, r.mx1}) {
            if (!chk(val, n.mn1, n.mx1)) continue;
            if (any != -1) {
                n.dgc = gcd(n.dgc, abs(val - any));
            } else {
                any = val;
            }
        }
    }

    void push_mineq(Node &n, ll x) {
        if (n.mx1 <= x) return;
        int fl = n.mn1 == n.mx1, fl2 = n.mn1 == n.mx2;
        n.sm -= n.cmx1 * (n.mx1 - x);
        n.mx1 = x;
        if (fl) {
            n.mn1 = x;
        } else if (fl2) {
            n.mn2 = x;
        }
    }

    void push_maxeq(Node &n, ll x) {
        if (n.mn1 >= x) return;
        int fl = n.mn1 == n.mx1, fl2 = n.mn1 == n.mx2;
        n.sm += n.cmn1 * (x - n.mn1);
        n.mn1 = x;
        if (fl) {
            n.mx1 = x;
        } else if (fl2) {
            n.mx2 = x;
        }
    }

    void push_add(int v, ll x) {
        Node &n = m[v];
        n.ps_add += x;
        n.mn1 += x;
        n.mx1 += x;
        n.mn2 += x;
        n.mx2 += x;
        n.sm += x * gsz(v);
    }

    void push(int v) {
        if (v >= U) return;
        Node &n = m[v], &l = m[v << 1], &r = m[v << 1 | 1];
        ll mn = n.mn1, mx = n.mx1;
        if (n.ps_add) {
            push_add(v << 1, n.ps_add);
            push_add(v << 1 | 1, n.ps_add);
            n.ps_add = 0;
        }
        push_mineq(l, mx);
        push_mineq(r, mx);
        push_maxeq(l, mn);
        push_maxeq(r, mn);
    }

    void seg_add(int ql, int qr, ll x, int l, int r, int v) {
        if (qr < l || r < ql) return;
        push(v);
        if (ql <= l && r <= qr) {
            push_add(v, x);
            return;
        }
        int md = (l + r) >> 1;
        seg_add(ql, qr, x, l, md, v << 1);
        seg_add(ql, qr, x, md + 1, r, v << 1 | 1);
        upd(v);
    }

    ll seg_gcd(int ql, int qr, int l, int r, int v) {
        Node &n = m[v];
        if (qr < l || r < ql) return 0;
        push(v);
        if (ql <= l && r <= qr) {
            ll o = n.dgc;
            if (n.mx1 != n.mn1) o = gcd(o, n.mx1 - n.mx2);
            if (n.mx1 != n.mn1) o = gcd(o, n.mn2 - n.mn1);
            o = gcd(o, n.mx1);
            return o;
        }
        int md = (l + r) >> 1;
        return gcd(seg_gcd(ql, qr, l, md, v << 1),
                   seg_gcd(ql, qr, md + 1, r, v << 1 | 1));
    }

    ll seg_sum(int ql, int qr, int l, int r, int v) {
        if (qr < l || r < ql) return 0;
        push(v);
        if (ql <= l && r <= qr) return m[v].sm;
        int md = (l + r) >> 1;
        return seg_sum(ql, qr, l, md, v << 1) +
               seg_sum(ql, qr, md + 1, r, v << 1 | 1);
    }

    ll seg_max(int ql, int qr, int l, int r, int v) {
        if (qr < l || r < ql) return -inf;
        push(v);
        if (ql <= l && r <= qr) return m[v].mx1;
        int md = (l + r) >> 1;
        return max(seg_max(ql, qr, l, md, v << 1),
                   seg_max(ql, qr, md + 1, r, v << 1 | 1));
    }

    ll seg_min(int ql, int qr, int l, int r, int v) {
        if (qr < l || r < ql) return inf;
        push(v);
        if (ql <= l && r <= qr) return m[v].mn1;
        int md = (l + r) >> 1;
        return min(seg_min(ql, qr, l, md, v << 1),
                   seg_min(ql, qr, md + 1, r, v << 1 | 1));
    }

    void seg_mineq(int ql, int qr, ll x, int l, int r, int v) {
        Node &n = m[v];
        if (qr < l || r < ql || x >= n.mx1) return;
        push(v);
        if (ql <= l && r <= qr && (n.mn1 == n.mx1 || x > n.mx2)) {
            push_mineq(n, x);
            return;
        }
        int md = (l + r) >> 1;
        seg_mineq(ql, qr, x, l, md, v << 1);
        seg_mineq(ql, qr, x, md + 1, r, v << 1 | 1);
        upd(v);
    }

    void seg_maxeq(int ql, int qr, ll x, int l, int r, int v) {
        Node &n = m[v];
        if (qr < l || r < ql || x <= n.mn1) return;
        push(v);
        if (ql <= l && r <= qr && (n.mn1 == n.mx1 || x < n.mn2)) {
            push_maxeq(n, x);
            return;
        }
        int md = (l + r) >> 1;
        seg_maxeq(ql, qr, x, l, md, v << 1);
        seg_maxeq(ql, qr, x, md + 1, r, v << 1 | 1);
        upd(v);
    }

    void seg_add(int ql, int qr, ll x) {seg_add(ql, qr, x, 0, U - 1, 1);}
    ll seg_gcd(int ql, int qr) {return seg_gcd(ql, qr, 0, U - 1, 1);}
    ll seg_sum(int ql, int qr) {return seg_sum(ql, qr, 0, U - 1, 1);}
    ll seg_max(int ql, int qr) {return seg_max(ql, qr, 0, U - 1, 1);}
    ll seg_min(int ql, int qr) {return seg_min(ql, qr, 0, U - 1, 1);}
    void seg_mineq(int ql, int qr, ll x) {seg_mineq(ql, qr, x, 0, U - 1, 1);}
    void seg_maxeq(int ql, int qr, ll x) {seg_maxeq(ql, qr, x, 0, U - 1, 1);}
    void seg_set(int ql, int qr, ll x) {seg_mineq(ql, qr, x); seg_maxeq(ql, qr, x);}
};