//All functions are uniformly random, unless other is stated
namespace Generator {

    const long double EPS = 1e-9;

    namespace impl {
        template<typename T>
        vector<array<T, 2>> convex_hull(vector<array<T, 2>> arr) {
            static const T eps = is_integral_v<T> ? 0 : EPS;
            if (arr.empty()) return {};
            auto cross = [](const array<T, 2>& p1, const array<T, 2>& p2) -> T {
                return p1[0] * p2[1] - p1[1] * p2[0];
            };
            array<T, 2> mnp = *min_element(arr.begin(), arr.end());
            for (auto& p : arr) p[0] -= mnp[0], p[1] -= mnp[1];
            sort(arr.begin(), arr.end(), [&](const array<T, 2>& p1, const array<T, 2>& p2) {
                T c = cross(p1, p2);
                return abs(c) > eps ? c > 0 : abs(p1[0]) + abs(p1[1]) < abs(p2[0]) + abs(p2[1]);
            });
            vector<array<T, 2>> ch;
            for (const auto& p : arr) {
                while (ch.size() > 1) {
                    auto p1 = ch.back();
                    p1[0] -= ch[ch.size() - 2][0];
                    p1[1] -= ch[ch.size() - 2][1];
                    auto p2 = p;
                    p2[0] -= ch.back()[0];
                    p2[1] -= ch.back()[1];
                    if (cross(p1, p2) > eps) break;
                    ch.pop_back();
                }
                if (ch.empty() || abs(p[0] - ch.back()[0]) + abs(p[1] - ch.back()[1]) > eps) {
                    ch.push_back(p);
                }
            }
            for (auto& p : ch) p[0] += mnp[0], p[1] += mnp[1];
            return ch;
        }

        array<size_t, 2> num_to_edge(uint64_t n, uint64_t c, bool is_directed) {
            if (is_directed) {
                size_t x = c / (n - 1), y = c % (n - 1);
                return {x, y + (y >= x)};
            }
            uint64_t D = 4 * n * n - 4 * n + 1 - 8 * c;
            size_t x = floor((2 * n - 3 - sqrtl(D)) / 2) + 1;
            size_t y = x + 1 + (c - (2 * n - x - 1) * x / 2);
            return {x, y};
        }

        uint64_t edge_to_num(uint64_t n, uint64_t x, uint64_t y, bool is_directed) {
            return is_directed ? x * (n - 1) + y - (y > x) : (n * 2 - 1 - x) * x / 2 + (y - x - 1);
        }
    }

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
    vector<T> gen_vector(size_t n, T l = numeric_limits<T>::min(), T r = numeric_limits<T>::max()) {
        vector<T> m(n);
        for (auto& i : m) i = gen_val<T>(l, r);
        return m;
    }

    //Complexity: O(n) expected
    template<typename T>
    vector<T> gen_vector_distinct(size_t n, T l = numeric_limits<T>::min(), T r = numeric_limits<T>::max()) {
        static_assert(is_integral_v<T>);
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
    vector<T> gen_vector_with_fixed_sum(size_t n, T l, T r, T sum) {
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
    vector<vector<T>> gen_matrix(size_t n, size_t m, T l = numeric_limits<T>::min(), T r = numeric_limits<T>::max()) {
        vector<vector<T>> res(n, vector<T>(m));
        for (auto& row : res) {
            for (auto& num : row) {
                num = gen_val<T>(l, r);
            }
        }
        return res;
    }

    string gen_alpha_string(size_t n, int first_letters = 26) {
        string s(n, 'A');
        for (auto& i : s) i += (gen_val<int>(0, 1) ? 32 : 0) + gen_val<int>(0, first_letters - 1);
        return s;
    }

    string gen_lowercase_string(size_t n, int first_letters = 26) {
        string s(n, 'a');
        for (auto& i : s) i += gen_val<int>(0, first_letters - 1);
        return s;
    }

    string gen_uppercase_string(size_t n, int first_letters = 26) {
        string s(n, 'A');
        for (auto& i : s) i += gen_val<int>(0, first_letters - 1);
        return s;
    }

    string gen_palindrome_lowercase_string(size_t n, int first_letters = 26) {
        string s = gen_lowercase_string(n / 2, first_letters);
        if (n & 1) s.push_back('a' + gen_val(0, first_letters - 1));
        string t = s;
        reverse(t.begin(), t.end());
        return s + t;
    }

    string gen_parenthesis_sequence(size_t n) {
        string s = string(n, '(');
        for (auto& c : s) c = gen_val<int>(0, 1) ? ')' : c;
        return s;
    }

    //Not uniformly random, P("((()))") < P("()()()")
    string gen_right_parenthesis_sequence(size_t n) {
        assert(n % 2 == 0);
        string s = string(n / 2, '(') + string(n / 2, ')');
        shuffle(s.begin(), s.end(), rngll);
        for (int q = 0, dep = 0; q < n; ++q) {
            dep += s[q] == '(' ? 1 : -1;
            if (dep < 0 || (dep == 0 && s[q] == '(')) s[q] = '(' + ')' - s[q];
        }
        return s;
    }

    string gen_fibonacci_string(size_t n, char a = 'a', char b = 'b') {
        string s; s += a, s += b;
        s.resize(n);
        for (size_t x = 1, y = 1; x + y < n; y += x, x = y - x) {
            std::copy(s.begin(), s.begin() + min(n - x - y, y), s.begin() + x + y);
        }
        return s;
    }

    string gen_thue_morse_string(size_t n, char a = 'a', char b = 'b') {
        string s(n, a);
        for (size_t i = 0; i < n; ++i) s[i] = __builtin_parityll(i) ? a : b;
        return s;
    }

    template<typename T>
    array<T, 2> gen_point_inside_circle(T cx, T cy, T r) {
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
    vector<array<size_t, 2>> gen_tree(size_t n) {
        assert(n > 0);
        vector<array<size_t, 2>> ans(n - 1);
        for (size_t q = 1; q < n; ++q) ans[q - 1] = {gen_val<size_t>(0, q - 1), q};
        vector<size_t> replacement(n);
        iota(replacement.begin(), replacement.end(), 0);
        shuffle(replacement.begin(), replacement.end(), rngll);
        for (auto& [x, y] : ans) {
            x = replacement[x];
            y = replacement[y];
            if (x > y) swap(x, y);
        }
        return ans;
    }

    //Uniformly random among all labeled trees
    //Complexity: O(n)
    vector<array<size_t, 2>> gen_tree_prufer(size_t n) {
        assert(n > 0);
        if (n == 1) return {};
        vector<size_t> pcode = gen_vector<size_t>(n - 2, 0, n - 1);
        vector<size_t> cnt(n);
        for (int v : pcode) ++cnt[v];
        vector<array<size_t, 2>> ans(n - 1);
        size_t leaf = find(cnt.begin(), cnt.end(), 0) - cnt.begin();
        for (int q = 0, ptr = leaf + 1; q < n - 2; ++q) {
            size_t p = pcode[q];
            ans[q] = {p, leaf};
            if (--cnt[p] == 0 && p < ptr) {
                leaf = p;
            } else {
                while (cnt[ptr]) ++ptr;
                leaf = ptr++;
            }
        }
        ans.back() = {leaf, n - 1};
        for (auto& [x, y] : ans) if (x > y) swap(x, y);
        return ans;
    }

    //Not uniformly random
    //Expected convex hull size: O(log(samples))
    //Complexity: O(samples * log(samples))
    template<typename T>
    vector<array<T, 2>> gen_convex_hull_inside_rectangle_monte_carlo(size_t samples, T x1, T y1, T x2, T y2) {
        assert(x1 <= x2 && y1 <= y2);
        vector<array<T, 2>> m(samples);
        for (auto& [x, y] : m) {
            x = gen_val(x1, x2);
            y = gen_val(y1, y2);
        }
        return impl::convex_hull(m);
    }

    //Not uniformly random
    //Expected convex hull size: O(cbrt(samples))
    //Complexity: O(samples * log(samples))
    template<typename T>
    vector<array<T, 2>> gen_convex_hull_inside_circle_monte_carlo(size_t samples, T cx, T cy, T r) {
        assert(r >= 0);
        vector<array<T, 2>> m(samples);
        for (auto& p : m) p = gen_point_inside_circle(cx, cy, r);
        return impl::convex_hull(m);
    }

    //https://cglab.ca/~sander/misc/ConvexGeneration/convex.html
    /*
    X = x2 - x1
    Y = y2 - y1
    T = min(X, Y, max(X, Y)^{2/3})
    Expected convex hull size:
        - ~samples when T is floating point type, depends on EPS
        - O(samples) when samples <= O(T) and T is integral type
        - o(T) when samples > O(T) and T is integral type
    */
    //Ensure that max(X * X, Y * Y, X * Y) fits in T
    //Complexity: O(samples * log(samples))
    //Note: max(X, Y) ^ 2 should fit in T
    template<typename T>
    vector<array<T, 2>> gen_convex_hull_inside_rectangle(size_t samples, T x1, T y1, T x2, T y2) {
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
        vector<array<T, 2>> ans(samples);
        for (size_t i = 0; i < samples; ++i) ans[i] = {dx[i], dy[i]};
        auto part = [](const array<T, 2>& p) -> int {
            if (p[1] < 0) return 0;
            if (p[1] == 0 && p[0] >= 0) return 1;
            return 2;
        };
        sort(ans.begin(), ans.end(), [&](const array<T, 2>& l, const array<T, 2>& r) {
            int pl = part(l), pr = part(r);
            if (pl != pr) return pl < pr;
            T cross = l[0] * r[1] - l[1] * r[0];
            if (cross) return cross > 0;
            return abs(l[0]) + abs(l[1]) < abs(r[0]) + abs(r[1]);
        });
        for (size_t i = 1; i < samples; ++i) {
            ans[i][0] += ans[i - 1][0];
            ans[i][1] += ans[i - 1][1];
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
        return impl::convex_hull(ans);
    }

    //Complexity: O(m)
    vector<array<size_t, 2>> gen_graph(size_t n, size_t m, bool is_directed) {
        assert(n || m == 0);
        vector<array<size_t, 2>> ans(m);
        for (auto& [x, y] : ans) {
            x = gen_val<size_t>(0, n - 1);
            y = gen_val<size_t>(0, n - 1);
            if (!is_directed && x > y) swap(x, y);
        }
        return ans;
    }

    //Complexity: O(m)
    vector<array<size_t, 2>> gen_graph_simple(size_t n, size_t m, bool is_directed) {
        const uint64_t MAX_E = 1ull * n * (n - 1) / (2 - is_directed);
        assert(m <= MAX_E);
        vector<array<size_t, 2>> ans(m);
        for (size_t i = 0; auto& c : gen_vector_distinct<uint64_t>(m, 0, MAX_E - 1)) {
            ans[i++] = impl::num_to_edge(n, c, is_directed);
        }
        return ans;
    }

    //Generates UNDIRECTED connected graph
    //Complexity: O(m)
    vector<array<size_t, 2>> gen_graph_simple_connected(size_t n, size_t m) {
        const uint64_t MAX_E = 1ull * n * (n - 1) / 2;
        assert(n <= m + 1 && m <= MAX_E);
        if (n < 2) return {};
        unordered_set<uint64_t> s(m);
        for (auto [x, y] : gen_tree_prufer(n)) {
            s.insert(impl::edge_to_num(n, x, y, 0));
        }
        for (auto c : gen_vector_distinct<uint64_t>(min((uint64_t)m + n - 1, MAX_E), 0, MAX_E - 1)) {
            if (s.size() == m) break;
            s.insert(c);
        }
        vector<array<size_t, 2>> ans(m);
        for (size_t i = 0; auto& c : s) {
            ans[i++] = impl::num_to_edge(n, c, 0);
        }
        return ans;
    }

    //Complexity: O(m)
    vector<array<size_t, 2>> gen_graph_simple_with_hamiltonian_path(size_t n, size_t m, bool is_directed) {
        const uint64_t MAX_E = 1ull * n * (n - 1) / (2 - is_directed);
        assert(n <= m + 1 && m <= MAX_E);
        if (n < 2) return {};
        unordered_set<uint64_t> s(m);
        vector<size_t> perm(n);
        iota(perm.begin(), perm.end(), 0);
        shuffle(perm.begin(), perm.end(), rngll);
        for (size_t i = 1; i < n; ++i) {
            auto x = perm[i - 1], y = perm[i];
            if (!is_directed && x > y) swap(x, y);
            s.insert(impl::edge_to_num(n, x, y, is_directed));
        }
        for (auto c : gen_vector_distinct<uint64_t>(min((uint64_t)m + n - 1, MAX_E), 0, MAX_E - 1)) {
            if (s.size() == m) break;
            s.insert(c);
        }
        vector<array<size_t, 2>> ans(m);
        for (size_t i = 0; auto& c : s) {
            ans[i++] = impl::num_to_edge(n, c, is_directed);
        }
        return ans;
    }

    //Generates all O(n) pythagorean triplets with hypothenuse <= n
    //O(n)
    template<typename T>
    vector<array<T, 3>> gen_all_pythagorean_triplets(T n) {
        vector<array<T, 3>> o;
        for (T q = 1; q * q <= n; ++q) {
            for (T w = q + 1; q * q + w * w <= n; ++w) {
                if (__gcd(q, w) != 1 || q % 2 && w % 2) continue;
                T a = w * w - q * q;
                T b = 2 * q * w;
                T c = q * q + w * w;
                if (a > b) swap(a, b);
                o.push_back({a, b, c});
                for (T k = 2; k * c <= n; ++k) {
                    o.push_back({k * a, k * b, k * c});
                }
            }
        }
        return o;
    }

    template<typename T_arr>
    auto gen_element_from(T_arr& m) {
        assert(!m.empty());
        return m[gen_val<size_t>(0, m.size() - 1)];
    }
}
using namespace Generator;
