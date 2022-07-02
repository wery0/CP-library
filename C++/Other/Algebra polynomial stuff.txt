struct fraction {
    ll num = 0;
    ll den = 1;

    void normalize() {
        if (den < 0) den *= -1, num *= -1;
        ll gc = num == 0 ? 1 : __gcd(abs(num), den);
        num /= gc;
        den /= gc;
    }

    fraction() {}

    fraction(ll a) {
        num = a;
    }

    fraction(ll a, ll b) {
        num = a;
        den = b;
        normalize();
    }

    bool is_integer() const {
        return den == 1;
    }

    fraction operator-() const {
        return {-num, den};
    }

    fraction operator+(const fraction &other) const {
        return {num * other.den + other.num * den, den * other.den};
    }

    void operator+=(const fraction &other) {
        num = num * other.den + other.num * den;
        den *= other.den;
    }

    fraction operator-(const fraction &other) const {
        return {num * other.den - other.num * den, den * other.den};
    }

    void operator-=(const fraction &other) {
        num = num * other.den - other.num * den;
        den *= other.den;
    }

    fraction operator*(const fraction &other) const {
        return {num * other.num, den * other.den};
    }

    void operator*=(const fraction &other) {
        num *= other.num;
        den *= other.den;
    }

    fraction operator/(const fraction &other) const {
        assert(other.num != 0);
        return {num * other.den, den * other.num};
    }

    void operator/=(const fraction &other) {
        assert(other.num != 0);
        num *= other.den;
        den *= other.num;
    }

    bool operator!=(const fraction &other) const {
        return num * other.den != other.num * den;
    }

    bool operator==(const fraction &other) const {
        return num * other.den == other.num * den;
    }

    bool operator<(const fraction &other) const {
        return num * other.den < other.num * den;
    }

    bool operator>(const fraction &other) const {
        return num * other.den > other.num * den;
    }

    friend ostream &operator<<(ostream &os, const fraction &fr) {
        if (fr.den == 1) return os << fr.num;
        return os << "\\frac{" << fr.num << "}{" << fr.den << "}";
    }

    friend fraction abs(const fraction &f) {
        return f > 0 ? f : -f;
    }

    friend ll gcd_of_numenators(const fraction &f1, const fraction &f2) {
        return __gcd(abs(f1.num), abs(f2.num));
    }

    friend fraction lcm_of_numenators(const fraction &f1, const fraction &f2) {
        if (f1 == 0 || f2 == 0) return abs(f1 + f2);
        ll gc = gcd_of_numenators(f1, f2);
        return fraction(abs(f1.num) / gc * abs(f2.num));
    }

    friend fraction inv(const fraction &f) {
        return {f.den, f.num};
    }

    friend int sign(const fraction &f) {
        return f.num < 0 ? -1 : f.num == 0 ? 0 : 1;
    }
};

struct var {
    char c;
    ll num;

    var() {}

    var(char cc, ll n) {
        c = cc;
        num = n;
    }

    bool operator==(const var &other) const {
        return c == other.c && num == other.num;
    }

    bool operator!=(const var &other) const {
        return c != other.c || num != other.num;
    }

    bool operator<(const var &other) const {
        tuple<char, ll> t1 = {c, num};
        tuple<char, ll> t2 = {other.c, other.num};
        return t1 < t2;
    }

    friend ostream &operator<<(ostream &os, const var &v) {
        if (v.num == 0) return os << v.c;
        return os << v.c << "_" << v.num;
    }
};

pll read_num(str &s, ll pos) {
    ll sgn = 1;
    if (s[pos] == '-' || s[pos] == '+') {
        if (s[pos] == '-') sgn = -1;
        ++pos;
    }
    assert(isdigit(s[pos]));
    ll ans = 0;
    for (; pos < s.size() && isdigit(s[pos]); ++pos) {
        ans = ans * 10 + s[pos] - '0';
    }
    return {ans * sgn, pos};
}

struct monomial {
    mutable fraction koef;
    map<var, ll> mp;

    monomial() {}

    monomial(fraction _koef, map<var, ll> _mp) {
        koef = _koef;
        mp = _mp;
    }

    monomial(str s) {
        //cout<<s<<endl;
        int l = 0;
        koef = 1;
        if ((s[0] == '+' || s[0] == '-') && s.size() > 1 && !isdigit(s[1])) {
            koef = s[0] == '+' ? 1 : -1;
            l = 1;
        } else if (isdigit(s[0]) || s[0] == '-' || s[0] == '+') {
            pll p = read_num(s, 0);
            koef = p.F;
            l = p.S;
        } else if (s[0] == '\\') {
            assert(s.substr(0, 6) == "\\frac{");
            pll p1 = read_num(s, 6);
            pll p2 = read_num(s, p1.S + 2);
            koef = fraction(p1.F, p2.F);
            l = p2.S + 1;
        }
        for (; l < s.size();) {
            int r = l;
//            while (r + 1 < s.size() && s[r + 1] != '_' && s[r + 1] != '^') {
//                ++r;
//            }
            assert(r - l + 1 == 1);
            char var_name = s[l];
            ll num = 0;
            if (r + 1 < s.size() && s[r + 1] == '_') {
                pll p = read_num(s, r + 2);
                num = p.F;
                r = p.S - 1;
            }
            ll pw = 1;
            if (r + 1 < s.size() && s[r + 1] == '^') {
                pll p = read_num(s, r + 2);
                pw = p.F;
                r = p.S - 1;
            }
            var v = var(var_name, num);
            if (!mp.count(v)) mp[v] = 0;
            mp[v] += pw;
            l = r + 1;
        }
    }

    friend ll deg(const monomial &m) {
        ll o = 0;
        for (auto &[v, pw]: m.mp) {
            o += pw;
        }
        return o;
    }

    bool empty() { return mp.empty(); }

    bool is_divisible_on(const monomial &other) const {
        assert(other.koef != 0);
        for (auto &[var, deg]: other.mp) {
            assert(deg > 0);
            if (!mp.count(var)) return 0;
            if (mp.at(var) < deg) return 0;
        }
        return 1;
    }

    monomial operator/(const monomial &other) const {
        assert(is_divisible_on(other));
        monomial res = monomial(koef / other.koef, mp);
        for (auto &[var, deg]: other.mp) {
            assert(deg > 0);
            res.mp[var] -= deg;
            if (res.mp[var] == 0) {
                res.mp.erase(var);
            }
        }
        return res;
    }

    bool operator==(const monomial &other) const {  //does't consider koefs
        if (mp.size() != other.mp.size()) return 0;
        auto it1 = mp.begin();
        auto it2 = other.mp.begin();
        for (; it1 != mp.end(); ++it1, ++it2) {
            if ((*it1) != (*it2)) return 0;
        }
        return 1;
    }

    bool operator!=(const monomial &other) const {  //does't consider koefs
        return !((*this) == other);
    }

    monomial operator*(const monomial &other) const {
        monomial res = monomial(koef * other.koef, mp);
        for (auto &[v, pw]: other.mp) {
            if (!res.mp.count(v)) res.mp[v] = 0;
            res.mp[v] += pw;
        }
        return res;
    }

    bool operator<(const monomial &other) const {
        auto it1 = mp.begin();
        auto it2 = other.mp.begin();
        for (; it1 != mp.end() && it2 != other.mp.end(); ++it1, ++it2) {
            if ((*it1) != (*it2)) {
                return (*it1).F != (*it2).F ? (*it1).F < (*it2).F : (*it1).S > (*it2).S;
            }
        }
        return mp.size() > other.mp.size();
    }

    monomial operator-() const {
        return monomial(-koef, mp);
    }

    friend monomial gcd(const monomial &m1, const monomial &m2) {
        monomial res = monomial(gcd_of_numenators(m1.koef, m2.koef), {});
        for (const auto &[v, pw]: m1.mp) {
            if (m2.mp.count(v)) res.mp[v] = min(pw, m2.mp.at(v));
        }
        return res;
    }

    friend monomial lcm(const monomial &m1, const monomial &m2) {
        monomial res = monomial(lcm_of_numenators(m1.koef, m2.koef), m1.mp);
        for (const auto &[v, pw]: m2.mp) {
            if (!res.mp.count(v)) res.mp[v] = pw;
            chmax(res.mp[v], pw);
        }
        return res;
    }

    friend ostream &operator<<(ostream &os, const monomial &mon) {
        fraction koef = mon.koef;
        assert(koef != 0);
        if (koef < 0) os << "-", koef *= -1;
        if (koef != 1 || mon.mp.empty()) os << koef;
        for (auto &[v, p]: mon.mp) {
            assert(p > 0);
            os << v;
            //if (p > 1) os << "^" << p;
            if (p > 1) os << "^{" << p << "}";
        }
        return os;
    }
};

struct polynomial {
    set<monomial> s;

    polynomial() {}

    polynomial(monomial m) {
        s.insert(m);
    }

    bool empty() const { return s.empty(); }

    void add(const monomial &m) {
        if (!s.count(m)) s.insert(m);
        else (*s.find(m)).koef += m.koef;
        if ((*s.find(m)).koef == 0) s.erase(m);
    }

    polynomial(str s) {
        s.erase(remove(all(s), ' '), s.end());
        s.erase(remove(all(s), '\n'), s.end());
        for (int l = 0; l < s.size();) {
            int r = l;
            while (r + 1 < s.size() && s[r + 1] != '-' && s[r + 1] != '+') ++r;
            add(monomial(s.substr(l, r - l + 1)));
            l = r + 1;
        }
        clear0();
    }

    polynomial operator*(const fraction &num) const {
        if (num == 0) return {};
        polynomial res;
        res.s = s;
        for (const auto &v: res.s) {
            v.koef *= num;
        }
        return res;
    }

    polynomial operator/(const fraction &num) const {
        return (*this) * inv(num);
    }

    void clear0() {
        vec<monomial> kek;
        for (auto &mon: s) {
            if (mon.koef == 0) kek.pb(mon);
        }
        for (auto &mon: kek) s.erase(mon);
    }

    polynomial operator+(const polynomial &other) const {
        polynomial res;
        res.s = s;
        for (const auto &mon: other.s) {
            res.add(mon);
        }
        res.clear0();
        return res;
    }

    void operator+=(const polynomial &other) {
        for (auto &mon: other.s) {
            add(mon);
        }
        clear0();
    }

    polynomial operator-(const polynomial &other) const {
        polynomial res;
        res.s = s;
        for (const auto &mon: other.s) {
            res.add(-mon);
        }
        res.clear0();
        return res;
    }

    void operator-=(const polynomial &other) {
        for (auto &mon: other.s) {
            add(-mon);
        }
        clear0();
    }

    polynomial operator*(const polynomial &other) const {
        polynomial res;
        for (const auto &mon1: s) {
            for (const auto &mon2: other.s) {
                res.add(mon1 * mon2);
            }
        }
        res.clear0();
        return res;
    }

    polynomial operator*(const monomial &other) const {
        polynomial res;
        for (const auto &mon1: s) {
            res.add(mon1 * other);
        }
        res.clear0();
        return res;
    }

    bool operator==(const polynomial &other) const {
        if (s.size() != other.s.size()) return 0;
        auto it1 = s.begin();
        auto it2 = other.s.begin();
        for (; it1 != s.end(); ++it1, ++it2) {
            if ((*it1) != (*it2)) return 0;
        }
        return 1;
    }

    bool operator!=(const polynomial &other) const {
        return !((*this) == other);
    }

//    bool operator<(const polynomial &other) const {
//        auto it1 = s.begin();
//        auto it2 = other.s.begin();
//        for (; it1 != s.end() && it2 != other.s.end(); ) {
//            if (*it1 != *it2) return *it1 < *it2;
//            it1 = next(it1);
//            it2 = next(it2);
//        }
//        return it1 == s.end() && it2 != other.s.end();
//    }

    monomial get_monom_divisible_on(const monomial &m) const {
        monomial ans;
        vec<monomial> ahaha;
        for (const auto &mon: s) {
            if (mon.is_divisible_on(m)) {
                ahaha.pb(mon);
            }
        }
        if (ahaha.empty()) return {};
        return ahaha[uid<int>(0, ahaha.size() - 1)(rnd)];
    }

    ll binpow(ll x, ll k, const ll mod = -1) {
        ll o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x;
            x = x * x;
            if (mod != -1) o %= mod, x %= mod;
        }
        return o;
    }

    ll integer_eval(umap<char, ll> vals, ll mod = -1) {
        ll res = 0;
        for (const auto &mon: s) {
            assert(mon.koef.is_integer());
            ll cur = mon.koef.num;
            if (mod != -1) cur %= mod;
            for (auto &[v, pw]: mon.mp) {
                ll tyt = binpow(vals.count(v.c) ? vals[v.c] : 0, pw, mod);
                cur = cur * tyt;
                if (mod != -1) cur %= mod;
            }
        }
        return res;
    }

    friend ll deg(const polynomial &p) {
        ll o = 0;
        for (auto &mon: p.s) chmax(o, deg(mon));
        return o;
    }

    friend monomial L(const polynomial &p) {
        return p.s.empty() ? monomial() : *p.s.begin();
    }

    friend polynomial S_poly(const polynomial &p1, const polynomial &p2) {
        monomial lc = lcm(L(p1), L(p2));
        monomial m1 = lc / L(p1);
        monomial m2 = lc / L(p2);
        //cout << "lcm(L("<<p1<<"), L("<<p2<<")) = " << lc << endl;
        //cout << "m_1 = " << m1 << endl;
        //cout << "m_2 = " << m2 << endl;
        return p1 * m1 - p2 * m2;
    }

    friend ostream &operator<<(ostream &os, const polynomial &pol) {
        int i = 0;
        if (pol.empty()) os << "0";
        for (const auto &mon: pol.s) {
            assert(mon.koef != 0);
            if (i > 0) {
                os << " ";
                if (mon.koef > 0) os << "+ " << mon;
                else os << "- " << (-mon);
            } else {
                os << mon;
            }
            ++i;
        }
        return os;
    }
};

polynomial reduce(const polynomial &g, const polynomial &f) {
    monomial m = g.get_monom_divisible_on(L(f));
    assert(!m.empty());
    monomial kek = m / L(f);
    return g - polynomial(kek) * f;
}

polynomial get_remainder(polynomial g, polynomial f) {
    int steps = 0;
    int flag = 0;
    for (int iter = 0; iter < 1000; ++iter) {
        monomial m = g.get_monom_divisible_on(L(f));
        if (m.empty()) {
            flag = 1;
            break;
        }
        steps += 1;
        monomial kek = m / L(f);
        polynomial nw = g - polynomial(kek) * f;
//        cout << "g: " << g << endl;
//        cout << "f: " << f << endl;
//        cout << "L(f): " << L(f) << endl;
//        cout << "kek: " << kek << endl;
//        cout << "m: " << m << endl;
//        cout << "res: " << nw << endl << endl;
        g = nw;
    }
    //cout<<"->>>>>>>>>>>>> "<<g<<endl<<endl;
    assert(flag == 1);
    return g;
}

polynomial ensure_remainder(polynomial g, polynomial f) {
    polynomial must = get_remainder(g, f);
    for (int i = 0; i < 100; ++i) {
        assert(get_remainder(g, f) == must);
    }
    return must;
}

polynomial system_reduce(polynomial g, vec<polynomial> &arr) {
    int flag = 0, steps = 0;
    for (int it = 0; it < 1000; it++) {
        int pos = -1;
        vec<int> good;
        for (int q = 0; q < arr.size(); q++) {
            monomial m = g.get_monom_divisible_on(L(arr[q]));
            if (!m.empty()) {
                pos = q;
                good.pb(q);
                //break;
            }
        }
        if (pos == -1) {
            if (!g.empty()) cout << " -> IRREDUCIBLE" << endl;
            flag = 1;
            break;
        }
        pos = good[0];
        ++steps;
        polynomial rem = reduce(g, arr[pos]);
        //cout<<"RED: f_"<<pos+1<<endl;
        if (it == 0) cout << g;
        cout << " \\stackrel{f_" << pos + 1 << "}{\\to}\\ " << rem;
        g = reduce(g, arr[pos]);
    }
    cout << endl;
    assert(flag == 1);
    return g;
}

bool is_redusable_to_zero(polynomial g, vec<polynomial> &arr) {
    return system_reduce(g, arr).empty();
}

bool is_grebner(vec<polynomial> &m) {
    for (int q = 0; q < m.size(); q++) {
        for (int w = q + 1; w < m.size(); w++) {
            polynomial s = S_poly(m[q], m[w]);
            //cout << "S(f_" << q + 1 << ", f_" << w + 1 << ") = " << s << endl;
            if (!is_redusable_to_zero(s, m)) return 0;
            //cout << endl;
        }
    }
    return 1;
}

vec<polynomial> get_grebner_basis(vec<polynomial> m) {
    for (int w = 1; w < m.size(); w++) {
        for (int q = 0; q < w; q++) {
            monomial gc = gcd(L(m[q]), L(m[w]));
            cout << "gcd(" << L(m[q]) << ", " << L(m[w]) << ") = " << gc << endl;
            if (gc == monomial("1")) {
                cout << "f_" << q + 1 << ",\\ f_" << w + 1 << ": gcd(L(f_" << q + 1 << "),\\ L(f_" <<
                     w + 1 << ")) = 1 \\Rightarrow S(f_" << q + 1 << ",\\ f_" << w + 1 << ") \\stackrel{F}{\\to} 0"
                     << endl;
                continue;
            }
            polynomial SS = S_poly(m[q], m[w]);
            polynomial r = system_reduce(SS, m);
            cout << "f_" << q + 1 << ",\\ f_" << w + 1 <<
                 ": S(f_" << q + 1 << ",\\ f_" << w + 1 << ") = " << SS << "\\ \\stackrel{f_" << "?" << "}{\\to}\\ "
                 << r << endl;
            if (!r.empty()) {
                m.pb(r);
                cout << "f_" << m.size() << " = " << r << endl;
            }
            cout << endl;
        }
    }
    return m;
}

vec<polynomial> erase_divisible_on_L(vec<polynomial> &m) {
    int cnt = 0;
    for (int q = 0; q < m.size(); q++) {
        for (int w = 0; w < m.size(); w++) {
            if (q == w) continue;
            if (L(m[q]).is_divisible_on(L(m[w]))) {
                m.erase(m.begin() + q);
                q--;
                break;
            }
        }
    }
    return m;
}

vec<polynomial> get_MRBG_from_GB(vec<polynomial> greb) {
    greb = erase_divisible_on_L(greb);
    for (int q = 0; q < greb.size(); q++) {
        vec<polynomial> other = greb;
        other.erase(other.begin() + q);
        polynomial nw = system_reduce(greb[q], other);
        if (nw != greb[q])cout << greb[q] << " --->>> " << nw << endl;
        greb[q] = nw;
    }
    for (int q = 0; q < greb.size(); q++) {
        greb[q] = greb[q] / fraction(L(greb[q]).koef);
    }
    return greb;
}

polynomial Zk_transform(polynomial p, int k) {
    polynomial res;
    for (monomial m: p.s) {
        while (m.koef < 0) m.koef += k;
        while (m.koef > k || m.koef == k) m.koef -= k;
        if (m.koef.den != 1 || m.koef.num % k) res += m;
    }
    return res;
}