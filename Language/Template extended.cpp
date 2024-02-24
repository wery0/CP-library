template<typename I> auto array_cnt(I f, I l) {umap<typename iterator_traits<I>::value_type, int> mp; while (f != l) ++mp[*f], ++f; return mp;}
template<typename I> auto subset_sum(I f, I l) {int a = l - f; vec<typename iterator_traits<I>::value_type> o(1 << a); for (int q = 1; q < (1 << a); ++q) {const int i = __builtin_ctz(q); o[q] = *(f + i) + o[q ^ (1 << i)];} return o;}
template<typename I> vec<pii> get_segs_of_eq_elems(I first, I last, function<bool(const typename iterator_traits<I>::value_type&, const typename iterator_traits<I>::value_type&)> cmp = [](const auto& l, const auto& r) {return l == r;}) {using T = typename iterator_traits<I>::value_type; vec<pii> ans; if (first == last) return ans; int l = 0, r = 1; T prev = *first; for (auto cit = next(first); cit != last; ++cit, ++r) {if (!cmp(*cit, prev)) {ans.emplace_back(l, r - 1); l = r;} prev = *cit;} ans.emplace_back(l, r - 1); return ans;}
template<typename I> int LCP(I f1, I l1, I f2, I l2) {for (int o = 0; ; ++f1, ++f2, ++o) if (f1 == l1 || f2 == l2 || *f1 != *f2) return o; return -1;}
template<typename I> int min_period(I f, I l) {int a = l - f; vec<int> m(a); for (int q = 1; q < a; ++q) {for (int w = m[q - 1]; w && !m[q]; w = m[w - 1]) {if (*(f + q) == *(f + w)) m[q] = w + 1;} m[q] += !m[q] && *(f + q) == *f;} int p = a - m.back(); return a % p ? a : p;}
template<typename I> bool is_palindrome(I f, I l) {for (--l; f < l; ++f, --l) if (*f != *l) return 0; return 1;}
str from_base_10_to_base_b(ll x, ll b) {str t; if (x == 0) t = "0"; for (; x; x /= b) t += (char)('0' + x % b); reverse(all(t)); return t;}
#define vi vec<int>
#define vl vec<ll>
#define vvi vec<vec<int>>
#define vvvi vec<vec<vec<int>>>
#define vvl vec<vec<ll>>
#define vpi vec<pii>
#define vpl vec<pll>
#define vs vec<str>
#define vvs vec<vec<str>>
const int dx[] = { -1, 0, 1, 0, -1, 1, 1, -1};
const int dy[] = {0, 1, 0, -1, 1, 1, -1, -1};
template<typename T> int sum_of_digits(T val) {int o = 0; for (; val; val /= 10) o += val % 10; return o;}
template<typename T> struct static_sum_query {vec<T> m; static_sum_query() = default; template<typename I>static_sum_query(I f, I l) {m.resize(l - f + 1); for (auto it = m.begin() + 1; f != l; ++f, ++it) {*it = *(it - 1) + *f;}} template<typename T_arr> static_sum_query(T_arr& m) {(*this) = static_sum_query(all(m));} inline T query(const int l, const int r) const {return m[r + 1] - m[l];}};
template<typename T> vec<pair<T, int>> zip_with_positions(vec<T> &m) {int a = m.size(); vec<pair<T, int>> ans(a); for (int q = 0; q < a; ++q) ans[q] = {m[q], q}; return ans;}
template<typename T> str join(vec<T> &m, str c) {str o; if constexpr(is_same<str, T>::value) {for (const T &s : m) o += s + c;} else {for (const T &s : m) o += to_string(s) + c;} if (o.size()) o.erase(o.end() - c.size(), o.end()); return o;}
vec<pii> get_reflection_points_in_rect(int a, int b, int x, int y) {assert(0 <= x && x < a); assert(0 <= y && y < b); vec<pii> res = {{x, y}}; if (x != a - x - 1) res.pb({a - x - 1, y}); if (y != a - y - 1) res.pb({x, a - y - 1}); if (x != a - x - 1 && y != a - y - 1) res.pb({a - x - 1, a - y - 1}); return res;}
vec<pii> get_rotation_points_in_square(int a, int x, int y) {assert(0 <= x && x < a); assert(0 <= y && y < a); vec<pii> res = {{x, y}}; if (a % 2 == 1 && x == a / 2 && y == a / 2) return res; res.pb({a - y - 1, x}); res.pb({a - x - 1, a - y - 1}); res.pb({y, a - x - 1}); return res;}
template<typename T> vec<vec<int>> get_cycles_of_perm(vec<T> &m, int permutation_indexation) {int a = m.size(); vec<vec<int>> ans; vec<bool> us(a); for (int q = 0; q < a; ++q) {if (us[q]) continue; vec<int> tyt; for (int w = q; !us[w]; w = m[w] - permutation_indexation) {tyt.pb(w); us[w] = 1;} ans.pb(tyt);} return ans;}
int find_closing_bracket(str &s, int i) {char op = s[i], cl = op == '(' ? ')' : op == '{' ? '}' : op == '[' ? ']' : op == '<' ? '>' : '@'; assert(cl != '@'); int dep = 1; for (int q = i + 1; q < s.size(); ++q) {dep += s[q] == op ? 1 : s[q] == cl ? -1 : 0; if (dep == 0) return q;} return -1;}
template<typename T> vec<pair<T, T>> vv_to_vp(vec<vec<T>> &m) {int a = m.size(); vec<pair<T, T>> ans(a); for (int q = 0; q < a; ++q) ans[q] = {m[q][0], m[q][1]}; return ans;}
template<const int k, typename T> vec<array<T, k>> vv_to_varr(vec<vec<T>> &m) {int a = m.size(); vec<array<T, k>> ans(a); for (int q = 0; q < a; ++q) for (int w = 0; w < k; ++w) ans[q][w] = m[q][w]; return ans;}
str from_base_10_to_base_b(str x, ll b) {return from_base_10_to_base_b(stoll(x), b);}
ll from_base_b_to_base_10(str x, ll b) {ll o = 0, pw = 1; for (int q = x.size() - 1; q >= 0; --q, pw *= b) o += (x[q] - '0') * pw; return o;}
str from_base_a_to_base_b(str x, ll a, ll b) {ll x10 = from_base_b_to_base_10(x, a); return from_base_10_to_base_b(x10, b);}
template<typename T> T binpow(T x, T k) {if (k < 0) return 0; T o = 1; for (; k; k >>= 1) {if (k & 1) o = o * x; x = x * x;} return o;}
template<typename T> T ar_prog_sum_fcl(T first, T cnt, T last) {return (first + last) * cnt / 2;}
template<typename T> T ar_prog_sum_fdc(T first, T diff, T cnt) {return (first * 2 + diff * (cnt - 1)) * cnt / 2;}
template<typename T> T ar_prog_sum_fdl(T first, T diff, T last) {return (first + last) * ((last - first) / diff + 1) / 2;}
template<typename T> T geom_prog_sum_fdl(T first, T diff, T last) {return (last * diff - first) / (diff - 1);}
template<typename T> T geom_prog_sum_fdc(T first, T diff, T cnt) {return (first * binpow(diff, cnt) - first) / (diff - 1);}
template<typename T> vec<vec<T>> transpose_matrix(vec<vec<T>> &m) {int a = m.size(), b = a ? m[0].size() : 0; vec<vec<T>> ans(b, vec<T>(a)); for (int q = 0; q < a; ++q) {for (int w = 0; w < b; ++w) {ans[w][q] = m[q][w];}} return ans;}
template<typename T> vec<vec<T>> rotate_matrix_cw(vec<vec<T>> &m) {int a = m.size(), b = a ? m[0].size() : 0; vec<vec<T>> ans(b, vec<T>(a)); for (int w = 0; w < b; ++w) for (int q = 0; q < a; ++q) ans[w][q] = m[a - 1 - q][w]; return ans;}
complex<ll> str_to_cmpl_ll(str t) {int ps = t.find('+'), sgn = 1; if (ps == string::npos) {ps = t.find('-'); sgn = -1; assert(ps != string::npos);} str t1 = t.substr(0, ps), t2 = t.substr(ps + 1); assert(t2.back() == 'i'); t2.pop_back(); return {stoll(t1), stoll(t2) * sgn};}
int time_to_minutes(int h, int m) {return h * 60 + m;}
int time_to_minutes(str s) {int ps = s.find(':'); assert(ps != string::npos); return time_to_minutes(stoi(s.substr(0, ps)), stoi(s.substr(ps + 1)));}
str minutes_to_time(int m, bool h0 = true, bool m0 = true) {int h = m / 60; m %= 60; str o; if (h0) o += (h < 10 ? "0" : ""); o += to_string(h); o += ':'; if (m0) o += (m < 10 ? "0" : ""); o += to_string(m); return o;}
bool is_vowel_lowercase(char c) {return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';}
bool is_consonant_lowercase(char c) {return !is_vowel_lowercase(c);}
ull leq_pow2ll(const ull x) {return 1ull << __lg(x);}
ull geq_pow2ll(const ull x) {return x & (x - 1) ? 2ull << __lg(x) : x;}
ll sqd(const pll p1, const pll p2) {return (p1.F - p2.F) * (p1.F - p2.F) + (p1.S - p2.S) * (p1.S - p2.S);}
ll sqd(const ll x1, const ll y1, const ll x2, const ll y2) {return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);}
template<typename T> int sign(T x) {return x < 0 ? -1 : x > 0 ? 1 : 0;}
template<typename I> bool is_subsequence(I f_pattern, I l_pattern, I f_text, I l_text) {for (; f_text != l_text && f_pattern != l_pattern; ++f_text) if (*f_text == *f_pattern) ++f_pattern; return f_pattern == l_pattern;}
vec<ll> get_divisors(ll x) {vec<ll> ans1, ans2; for (ll q = 1; q * q <= x; ++q) {if (x % q == 0) {ans1.pb(q); ans2.pb(x / q);}} if (ans1.back() == ans2.back()) ans1.pop_back(); reverse(all(ans2)); for (ll i : ans2) ans1.pb(i); return ans1;}
bool is_prime(ll c) {if (c < 2) return 0; if (c == 2 || c == 3) return 1; if (c % 2 == 0 || c % 3 == 0) return 0; const ll gr = sqrtl(c) + 1; for (ll q = 6; q <= gr; q += 6) {if (c % (q - 1) == 0) return 0; if (c % (q + 1) == 0) return 0;} return 1;}
vec<str> split(str &s, char c, bool ignore_empty = false) {vec<str> o; str u; for (int q = 0; q < s.size(); q++) {if (s[q] == c) {if (!u.empty() || !ignore_empty) o.pb(u); u.clear();} else u += s[q];} if (!u.empty() || !ignore_empty) o.pb(u); return o;}
int replace(str &s, str from, str to) {str t = from; t.pb(0); t += s; int a = t.size(); vec<int> m(a); for (int q = 1; q < a; ++q) {for (int w = m[q - 1]; w && !m[q]; w = m[w - 1]) {if (t[q] == t[w]) m[q] = w + 1;} m[q] += !m[q] && t[q] == t[0];} int szf = from.size(), lst = szf; for (int q = szf; q < t.size(); ++q) {if (m[q] == szf && q - lst >= szf) {m[q - szf + 1] = -1; lst = q;}} str ans; int o = 0; for (int q = szf + 1; q < t.size(); ++q) {if (m[q] != -1) ans += t[q]; else ans += to, q += szf - 1, ++o;} s = ans; return o;}
template<typename T> T mul_threshold(T a, T b, T threshold) {if (!a || !b || !threshold) return 0; assert(a > 0 && b > 0); T max_b = threshold / a; return b <= max_b ? a * b : threshold;}
template<typename T> T count_set_bits_pref(T n, int b) {assert(0 <= n); T pw = (T)(1) << b; if (pw > n) return 0; T period = pw * 2; T full = (n + 1) / period; T rem = (n + 1) & (period - 1); return full * pw + (rem < pw ? 0 : rem - pw);}
template<typename T> T count_set_bits_seg(T l, T r, int b) {assert(0 <= l && l <= r); return count_set_bits_pref(r, b) - (l ? count_set_bits_pref(l - 1, b) : 0);}
template<typename I> vector<int> get_substring_occurrences(I f_pattern, I l_pattern, I f_text, I l_text) {const size_t n = std::distance(f_pattern, l_pattern); const size_t m = std::distance(f_text, l_text); assert(n); auto cmp = [&](size_t i, size_t j) {if (i == n || j == n) return i == j; return (i < n ? *(f_pattern + i) : *(f_text + i - n - 1)) == (j < n ? * (f_pattern + j) : * (f_text + j - n - 1));}; vector<int> pf(n), res; for (size_t i = 1, ppf = 0; i < n + 1 + m; ++i) {size_t p = 0; for (size_t j = ppf; j && !p; j = pf[j - 1]) {if (cmp(i, j)) p = j + 1;} p += !p && cmp(i, 0); ppf = p; if (i < n) pf[i] = p; if (p == n) res.push_back(i - n * 2);} return res;}
template<typename T> T integral_tersearch_argmin(auto f, T l, T r) {static_assert(is_integral_v<decltype(l)>); using U = decltype(f(l)); const ld FI = 1.6180339887498948482045868343656381177203; T p1 = l + (r - l) / (FI + 1), p2 = r - (r - l) / (FI + 1); U v1 = f(p1), v2 = f(p2); while (r - l > 7) {if (v1 < v2) {r = p2; p2 = p1, v2 = v1; p1 = l + (r - l) / (FI + 1), v1 = f(p1);} else {l = p1; p1 = p2, v1 = v2; p2 = r - (r - l) / (FI + 1), v2 = f(p2);}} T best_arg = l; U best_val = f(l), prv = best_val; while (++l <= r) {U tyt = l == p1 ? v1 : l == p2 ? v2 : f(l); if (tyt > prv) break; if (chmin(best_val, tyt)) best_arg = l; prv = tyt;} return best_arg;};
template<typename T> T integral_binary_search_last_when_true(auto f, T l, T r) {static_assert(is_same<decltype(l), decltype(r)>::value); while (l + 1 < r) {T md = l + (r - l) / 2; if (f(md))l = md; else r = md;} return l;};
template<typename T> T integral_binary_search_last_when_true(auto f, T l) {T d = 1; while (f(l + d)) d *= 2; d /= 2; for (T u = d; u; u /= 2) if (f(l + d + u)) d += u; return l + d;};
