//Just run it on the same compiler and same options as the solution you want to hack.
//Works for integral types, and std::string. Slow for std::string.
template<typename T>
vector<T> unordered_hacker(const size_t N) {
    auto get_bucket_counts = [&]() -> vector<int> {
        vector<int> ans;
        unordered_set<int> s;
        uniform_int_distribution<int> gen(0, INT32_MAX);
        while (s.size() < N) {
            if (ans.empty() || ans.back() != s.bucket_count()) {
                ans.push_back(s.bucket_count());
            }
            s.insert(gen(rnd));
        }
        return ans;
    };
    vector<int> bc = get_bucket_counts();
    if constexpr(is_same_v<string, T>) {
        //Edit these, if need
        const size_t len = 15;
        const string pref = "";
        const string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
        
        uniform_int_distribution<int> gen(0, alphabet.size() - 1);
        auto gen_str_with_divisible_hash = [&](int mod) -> string {
            string s = pref;
            while (true) {
                s.resize(pref);
                while (s.size() < len) {
                    s += alphabet[gen(rnd)];
                }
                if (hash<string>{}(s) % mod == 0) return s;
            }
            return "";
        };
        vector<T> ans1; int64_t op1 = 0;
        for (size_t i = 0, psb = 0, cnt = 0, lst = bc[0]; i < N; ++i) {
            T nw = gen_str_with_divisible_hash(lst);
            ans1.push_back(nw);
            op1 += cnt;
            if (i >= lst) {
                lst = bc[++psb];
                cnt = 1;
            } else ++cnt;
        }
        vector<T> ans2; int64_t op2 = 0;
        for (size_t i = 0, cnt = 0, pr = bc.size() > 1 ? bc[bc.size() - 2] : 0, lst = bc.back(); i < N; ++i) {
            T nw = gen_str_with_divisible_hash(lst);
            ans2.push_back(nw);
            op2 += i < pr ? 1 : cnt;
            ++cnt;
        }
        return op1 > op2 ? ans1 : ans2;
    } else if constexpr(is_integral_v<T>) {
        const T mx = numeric_limits<T>::max();
        vector<T> ans1; int64_t op1 = 0;
        for (size_t i = 0, psb = 0, cnt = 0, lst = bc[0]; i < N; ++i) {
            ans1.push_back(lst * cnt);
            op1 += cnt;
            if (i >= lst) {
                lst = bc[++psb];
                cnt = 1;
            } else ++cnt;
        }
        vector<T> ans2; int64_t op2 = 0;
        for (size_t i = 0, cnt = 0, pr = bc.size() > 1 ? bc[bc.size() - 2] : 0, lst = bc.back(); i < N; ++i) {
            ans2.push_back(lst * cnt);
            op2 += i < pr ? 1 : cnt;
            ++cnt;
        }
        return op1 > op2 ? ans1 : ans2;
    }
    assert(0);
    return {};
}
