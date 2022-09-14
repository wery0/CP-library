str gen_str_with_divisible_hash(int len, int mod, str alphabet, str pref = "") {
    uid<int> gen(0, isz(alphabet) - 1);
    for (;;) {
        str s = pref;
        while (isz(s) < len) {
            s += alphabet[gen(rnd)];
        }
        if (hash<str> {}(s) % mod == 0) return s;
    }
    return "LOX";
}

//Just run it on the same compiler and same options as the solution you want to hack.
template<typename T>
vec<T> uset_hacker(const int N) {
    auto get_bucket_counts = [&]() -> vec<int> {
        vec<int> ans;
        unordered_set<int> s;
        uid<int> gen(0, INT32_MAX);
        for (; s.size() < N;) {
            if (ans.empty() || ans.back() != s.bucket_count()) {
                ans.pb(s.bucket_count());
            }
            s.insert(gen(rnd));
        }
        return ans;
    };
    vec<int> bc = get_bucket_counts();
    if constexpr(is_same<string, T>::value) {
        const int len = 15;
        const str pref = "";
        const str alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
        vec<T> ans1; ll op1 = 0;
        for (ll i = 0, psb = 0, cnt = 0, lst = bc[0]; i < N; ++i) {
            T nw = gen_str_with_divisible_hash(len, lst, alphabet, pref);
            ans1.pb(nw);
            op1 += cnt;
            if (i >= lst) {
                lst = bc[++psb];
                cnt = 1;
            } else ++cnt;
        }
        vec<T> ans2; ll op2 = 0;
        for (ll i = 0, cnt = 0, pr = isz(bc) > 1 ? bc[isz(bc) - 2] : 0, lst = bc.back(); i < N; ++i) {
            T nw = gen_str_with_divisible_hash(len, lst, alphabet, pref);
            ans2.pb(nw);
            op2 += i < pr ? 1 : cnt;
            ++cnt;
        }
        return op1 > op2 ? ans1 : ans2;
    } else {
        const T mx = numeric_limits<T>::max();
        vec<T> ans1; ll op1 = 0;
        for (ll i = 0, psb = 0, cnt = 0, lst = bc[0]; i < N; ++i) {
            ans1.pb(lst * cnt);
            op1 += cnt;
            if (i >= lst) {
                lst = bc[++psb];
                cnt = 1;
            } else ++cnt;
        }
        vec<T> ans2; ll op2 = 0;
        for (ll i = 0, cnt = 0, pr = isz(bc) > 1 ? bc[isz(bc) - 2] : 0, lst = bc.back(); i < N; ++i) {
            ans2.pb(lst * cnt);
            op2 += i < pr ? 1 : cnt;
            ++cnt;
        }
        return op1 > op2 ? ans1 : ans2;
    }
}