//All functions are uniformly random, unless other is stated
namespace Generator {

    mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    mt19937_64 rngll(std::chrono::steady_clock::now().time_since_epoch().count());

    template<typename T>
    T gen_val(T l = numeric_limits<T>::min(), T r = numeric_limits<T>::max()) {
        if constexpr(is_integral_v<T>) {
            uniform_int_distribution<T> gint(l, r);
            return gint(rngll);
        } else if constexpr(is_floating_point_v<T>) {
            uniform_real_distribution<T> greal(l, r);
            return greal(rngll);
        } else {
            assert(0);
        }
        return 0;
    }

    template<typename T>
    vector<T> gen_vector(int n, T l = numeric_limits<T>::min(), T r = numeric_limits<T>::max()) {
        vector<T> m(n);
        for (auto& i : m) i = gen_val<T>(l, r);
        return m;
    }

    //Complexity: O(n) expected
    template<typename T>
    vector<T> gen_vector_distinct(int n, T l = numeric_limits<T>::min(), T r = numeric_limits<T>::max()) {
        T d = r - l + 1;
        assert(n <= d);
        if (n * 2 <= d) {
            vector<T> m(n);
            unordered_set<T> s(n);
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
        shuffle(m.begin(), m.end(), rngll);
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
    vector<vector<T>> gen_matrix(int n, int m, T l = numeric_limits<T>::min(), T r = numeric_limits<T>::max()) {
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
        shuffle(s.begin(), s.end(), rngll);
        for (int q = 0, dep = 0; q < n; ++q) {
            dep += s[q] == '(' ? 1 : -1;
            if (dep < 0 || (dep == 0 && s[q] == '(')) s[q] = '(' + ')' - s[q];
        }
        return s;
    }

    template<typename T>
    pair<T, T> gen_point_inside_circle(T cx, T cy, T r) {
        T x = gen_val(cx - r, cx + r);
        T y = gen_val(cy - r, cy + r);
        while ((cx - x) * (cx - x) + (cy - y) * (cy - y) > r * r) {
            x = gen_val(cx - r, cx + r);
            y = gen_val(cy - r, cy + r);
        }
        return {x, y};
    }

    //Not uniformly random
    //Complexity: O(n)
    vector<pair<int, int>> gen_tree(int n) {
        assert(n > 0);
        vector<pair<int, int>> ans(n - 1);
        for (int q = 1; q < n; ++q) ans[q - 1] = {gen_val(0, q - 1), q};
        vector<int> replacement(n);
        iota(replacement.begin(), replacement.end(), 0);
        shuffle(replacement.begin(), replacement.end(), rngll);
        for (auto& [x, y] : ans) {
            x = replacement[x];
            y = replacement[y];
        }
        return ans;
    }

    //Uniformly random among all labeled trees
    //Complexity: O(n)
    vector<pair<int, int>> gen_tree_prufer(int n) {
        assert(n > 0);
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
        sort(arr.begin(), arr.end(), [&](const auto& p1, const auto& p2) {
            T c = cross(p1, p2);
            return c ? c > 0 : abs(p1.first) + abs(p1.second) < abs(p2.first) + abs(p2.second);
        });
        if (arr[0] == arr.back()) return {arr[0]};
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

    //Not uniformly random
    //Expected convex hull size: O(log(samples))
    //Complexity: O(samples * log(samples))
    template<typename T>
    vector<pair<T, T>> gen_convex_hull_inside_rectangle_monte_carlo(size_t samples, T x1, T y1, T x2, T y2) {
        assert(x1 <= x2 && y1 <= y2);
        vector<pair<T, T>> m(samples);
        for (auto& [x, y] : m) {
            x = gen_val(x1, x2);
            y = gen_val(y1, y2);
        }
        return __convex_hull(m);
    }

    //Not uniformly random
    //Expected convex hull size: O(cbrt(samples))
    //Complexity: O(samples * log(samples))
    template<typename T>
    vector<pair<T, T>> gen_convex_hull_inside_circle_monte_carlo(size_t samples, T cx, T cy, T r) {
        assert(r >= 0);
        vector<pair<T, T>> m(samples);
        for (auto& p : m) p = gen_point_inside_circle(cx, cy, r);
        return __convex_hull(m);
    }

    //https://cglab.ca/~sander/misc/ConvexGeneration/convex.html
    /*
    X = x2 - x1
    Y = y2 - y1
    T = min(X, Y, max(X, Y)^{2/3})
    Expected convex hull size:
        - O(samples) when samples <= O(T)
        - o(T) when samples > O(T)
    */
    //Complexity: O(samples * log(samples))
    //Note: max(X, Y) ^ 2 should fit in T
    template<typename T>
    vector<pair<T, T>> gen_convex_hull_inside_rectangle(size_t samples, T x1, T y1, T x2, T y2) {
        assert(x1 <= x2 && y1 <= y2);
        if (samples == 0) return {};
        if (samples == 1) return {{gen_val(x1, x2), gen_val(y1, y2)}};
        vector<T> px = gen_vector(samples, x1, x2);
        vector<T> py = gen_vector(samples, y1, y2);
        sort(px.begin(), px.end());
        sort(py.begin(), py.end());
        vector<T> dx(samples), dy(samples);
        T lx[2] = {px[0], px[0]};
        T ly[2] = {py[0], py[0]};
        for (size_t i = 1; i < samples - 1; ++i) {
            int cx = gen_val(0, 1);
            dx[i - 1] = (px[i] - lx[cx]) * (cx ? -1 : 1);
            lx[cx] = px[i];
        }
        for (size_t i = 1; i < samples - 1; ++i) {
            int cy = gen_val(0, 1);
            dy[i - 1] = (py[i] - ly[cy]) * (cy ? -1 : 1);
            ly[cy] = py[i];
        }
        dx[samples - 2] = px[samples - 1] - lx[0];
        dx[samples - 1] = lx[1] - px[samples - 1];
        dy[samples - 2] = py[samples - 1] - ly[0];
        dy[samples - 1] = ly[1] - py[samples - 1];
        shuffle(dy.begin(), dy.end(), rngll);
        vector<pair<T, T>> ans(samples);
        for (size_t i = 0; i < samples; ++i) ans[i] = {dx[i], dy[i]};
        auto part = [](const pair<T, T>& p) -> int {
            if (p.second < 0) return 0;
            if (p.second == 0 && p.first >= 0) return 1;
            return 2;
        };
        sort(ans.begin(), ans.end(), [&](const auto& l, const auto& r) {
            int pl = part(l), pr = part(r);
            if (pl != pr) return pl < pr;
            T cross = l.first * r.second - l.second * r.first;
            if (cross) return cross > 0;
            return abs(l.first) + abs(l.second) < abs(r.first) + abs(r.second);
        });
        for (size_t i = 1; i < samples; ++i) {
            ans[i].first += ans[i - 1].first;
            ans[i].second += ans[i - 1].second;
        }
        T mnx = numeric_limits<T>::max(), mxx = numeric_limits<T>::min();
        T mny = numeric_limits<T>::max(), mxy = numeric_limits<T>::min();
        for (const auto& [x, y] : ans) {
            mnx = min(mnx, x), mxx = max(mxx, x);
            mny = min(mny, y), mxy = max(mxy, y);
        }
        T ax = x1 - mnx, ay = y1 - mny;
        ax += gen_val<T>(0, x2 - (mxx + ax));
        ay += gen_val<T>(0, y2 - (mxy + ay));
        for (auto& [x, y] : ans) x += ax, y += ay;
        return __convex_hull(ans);
    }

    template<typename T_arr>
    auto gen_element_from(T_arr& m) {
        assert(!m.empty());
        return m[gen_val<size_t>(0, m.size() - 1)];
    }
}
using namespace Generator;