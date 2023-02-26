//All functions are uniformly random, unless other is stated
namespace Generator {

#define nlmin numeric_limits<T>::min() / 2
#define nlmax numeric_limits<T>::max() / 2

    template<typename T>
    T gen_val(T l = nlmin, T r = nlmax) {
        if constexpr(is_integral_v<T>) {
            uniform_int_distribution<T> gint(l, r);
            return gint(rndll);
        } else if constexpr(is_floating_point_v<T>) {
            uniform_real_distribution<T> greal(l, r);
            return greal(rndll);
        } else {
            assert(0);
        }
        return 0;
    }

    template<typename T>
    vector<T> gen_vector(int n, T l = nlmin, T r = nlmax) {
        vector<T> m(n);
        for (auto& i : m) i = gen_val<T>(l, r);
        return m;
    }

    template<typename T>
    vector<T> gen_vector_distinct(int n, T l = nlmin, T r = nlmax) {
        T d = r - l + 1;
        assert(n <= d);
        if (n * 2 <= d) {
            vector<T> m(n);
            uset<T> s(n);
            for (auto& i : m) {
                T c = gen_val<T>(l, r);
                while (s.count(c)) c = gen_val<T>(l, r);
                s.insert(c);
                i = c;
            }
            return m;
        }
        vector<T> m(d);
        iota(m.begin(), m.end(), l);
        shuffle(m.begin(), m.end(), rndll);
        m.resize(n);
        return m;
    }

    //Not uniformly random
    /*
      len = r - l, my = sum / n - l
      Complexity: O(n * log(len / (len - abs(len / 2 - my))))
    */
    template<typename T>
    vector<T> gen_vector_with_fixed_sum(T n, T l, T r, T sum) {
        assert(l * n <= sum && sum <= r * n);
        vector<T> res(n);
        for (T& c : res) c = gen_val(l, r);
        T cur = accumulate(res.begin(), res.end(), (T)0);
        while (cur != sum) {
            if (cur < sum) {
                for (T& c : res) {
                    T eso = r - c;
                    T tyt = min(gen_val<T>(0, eso), sum - cur);
                    c += tyt;
                    cur += tyt;
                }
            } else {
                for (T& c : res) {
                    T eso = c - l;
                    T tyt = min(gen_val<T>(0, eso), cur - sum);
                    c -= tyt;
                    cur -= tyt;
                }
            }
        }
        return res;
    }

    template<typename T>
    vector<vector<T>> gen_matrix(int n, int m, T l = nlmin, T r = nlmax) {
        vector<vector<T>> res(n, vector<T>(m));
        for (auto& row : res) {
            for (auto& num : row) {
                num = gen_val<T>(l, r);
            }
        }
        return res;
    }

    string gen_alpha_string(int n, int first_letters = 26) {
        string s(n, 'A');
        for (auto& i : s) i += (gen_val<int>(0, 1) ? 32 : 0) + gen_val<int>(0, first_letters - 1);
        return s;
    }

    string gen_lowercase_string(int n, int first_letters = 26) {
        string s(n, 'a');
        for (auto& i : s) i += gen_val<int>(0, first_letters - 1);
        return s;
    }

    string gen_uppercase_string(int n, int first_letters = 26) {
        string s(n, 'A');
        for (auto& i : s) i += gen_val<int>(0, first_letters - 1);
        return s;
    }

    string gen_palindrome_lowercase_string(int n, int first_letters = 26) {
        string s = gen_lowercase_string(n / 2, first_letters);
        if (n & 1) s.push_back('a' + gen_val(0, first_letters - 1));
        string t = s;
        reverse(t.begin(), t.end());
        return s + t;
    }

    string gen_parenthesis_sequence(int n) {
        string s = string(n, '(');
        for (auto& c : s) c = gen_val<int>(0, 1) ? ')' : c;
        return s;
    }

    //Not uniformly random, P("((()))") < P("()()()")
    string gen_right_parenthesis_sequence(int n) {
        assert(n % 2 == 0);
        string s = string(n / 2, '(') + string(n / 2, ')');
        shuffle(s.begin(), s.end(), rnd);
        for (int q = 0, dep = 0; q < n; ++q) {
            dep += s[q] == '(' ? 1 : -1;
            if (dep < 0 || (dep == 0 && s[q] == '(')) s[q] = '(' + ')' - s[q];
        }
        return s;
    }

    //Not uniformly random
    vector<pair<int, int>> gen_tree(int n) {
        vector<pair<int, int>> ans(n - 1);
        for (int q = 1; q < n; ++q) {
            ans[q - 1] = {gen_val(0, q - 1), q};
        }
        vector<int> replacement(n);
        iota(replacement.begin(), replacement.end(), 0);
        shuffle(replacement.begin(), replacement.end(), rnd);
        for (auto& [x, y] : ans) {
            x = replacement[x];
            y = replacement[y];
        }
        return ans;
    }

    //Uniformly random among all labeled trees
    vector<pair<int, int>> gen_tree_prufer(int n) {
        if (n == 1) return {};
        vector<int> pcode = gen_vector(n - 2, 0, n - 1);
        vector<int> cnt(n);
        for (int v : pcode) ++cnt[v];
        vector<pair<int, int>> ans(n - 1);
        int leaf = find(cnt.begin(), cnt.end(), 0) - cnt.begin();
        for (int q = 0, ptr = leaf + 1; q < n - 2; ++q) {
            int p = pcode[q];
            ans[q] = {p, leaf};
            if (--cnt[p] == 0 && p < ptr) {
                leaf = p;
            } else {
                while (cnt[ptr]) ++ptr;
                leaf = ptr++;
            }
        }
        ans.back() = {leaf, n - 1};
        return ans;
    }

    template<typename T>
    vector<pair<T, T>> __convex_hull(vector<pair<T, T>> arr) {
        if (arr.empty()) return {};
        auto cross = [](const pair<T, T>& p1, const pair<T, T>& p2) -> T {
            return p1.first * p2.second - p1.second * p2.first;
        };
        pair<T, T> mnp = *min_element(arr.begin(), arr.end());
        for (auto& p : arr) p.first -= mnp.first, p.second -= mnp.second;
        sort(arr.begin(), arr.end(), [&](const auto & p1, const auto & p2) {
            T c = cross(p1, p2);
            return c ? c > 0 : abs(p1.first) + abs(p1.second) < abs(p2.first) + abs(p2.second);
        });
        vector<pair<T, T>> ch;
        for (int i = 0; i < arr.size(); ++i) {
            while (ch.size() > 1) {
                auto p1 = ch.back();
                p1.first -= ch[ch.size() - 2].first;
                p1.second -= ch[ch.size() - 2].second;
                auto p2 = arr[i];
                p2.first -= ch.back().first;
                p2.second -= ch.back().second;
                if (cross(p1, p2) > 0) break;
                ch.pop_back();
            }
            ch.push_back(arr[i]);
        }
        for (auto& p : ch) p.first += mnp.first, p.second += mnp.second;
        return ch;
    }

    //Expected convex hull size: O(log(n))
    template<typename T>
    vector<pair<T, T>> gen_convex_hull_inside_rectangle(int n, T x1, T y1, T x2, T y2) {
        assert(x1 <= x2 && y1 <= y2);
        vector<pair<T, T>> m(n);
        for (auto& [x, y] : m) {
            x = gen_val(x1, x2);
            y = gen_val(y1, y2);
        }
        return __convex_hull(m);
    }

    //Expected convex hull size: O(cbrt(n))
    template<typename T>
    vector<pair<T, T>> gen_convex_hull_inside_circle(int n, T cx, T cy, T r) {
        assert(r >= 0);
        vector<pair<T, T>> m(n);
        for (int i = 0; i < n; ++i) {
            T x = gen_val(cx - r, cx + r);
            T y = gen_val(cy - r, cy + r);
            if ((x - cx) * (x - cx) + (y - cy) * (y - cy) > r * r) --i;
            else m[i] = {x, y};
        }
        return __convex_hull(m);
    }

    template<typename T_arr>
    auto gen_element_from(T_arr& m) {
        assert(!m.empty());
        return m[gen_val<size_t>(0, m.size() - 1)];
    }
}
using namespace Generator;