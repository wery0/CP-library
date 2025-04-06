/*
  Capable of solving big (10^5+ variables) sparse systems of linear equations in (sometimes) mere milliseconds
  W:
    https://codeforces.com/contest/1823/problem/F
    https://codeforces.com/contest/2028/problem/E
    https://codeforces.com/contest/2032/problem/E
    https://codeforces.com/contest/2055/problem/C
    https://judge.yosupo.jp/problem/sparse_matrix_det
    https://codeforces.com/contest/1844/problem/G (smart approach with 57 SLEs)
  L:
    https://codeforces.com/contest/1844/problem/G (straightforward approach)
*/
enum EMatrixContainer {MAP, UMAP, LINEAR_UMAP, UMAP_SWITCHER};
enum EColumnContainer {SET, USET, LINEAR_USET, USET_SWITCHER};
template<
        typename T,
        bool min_col = false,
        EMatrixContainer MatrixContainer = UMAP,
        EColumnContainer ColumnContainer = USET_SWITCHER,
        template<typename> class Allocator = std::allocator
        >
class sparse_gauss {

#define vecall(K) vector<K, Allocator<K>>

    struct segtree_point_upd {

        size_t n;
        vecall(int) mn;

        segtree_point_upd(size_t n): n(n), mn(n * 2) {}

        size_t pos_of_min() const {
            size_t v = 1;
            while (v < n) v <<= 1, v |= mn[v] > mn[v | 1];
            return v - n;
        }

        void point_add(size_t i, int val) {
            i += n;
            mn[i] += val;
            for (i >>= 1; i; i >>= 1) {
                auto tmp = min(mn[i << 1], mn[i << 1 | 1]);
                if (tmp == mn[i]) break;
                mn[i] = tmp;
            }
        }

        vector<int> pos_of_min_k(size_t k) {
            if (k == 1) return {(int)pos_of_min()};
            assert(k && k <= n);
            auto cmp = [&](int x, int y) {return mn[x] > mn[y];};
            static priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
            vector<int> res(k);
            pq.emplace(1);
            for (size_t i = 0; i < k; ++i) {
                size_t v = pq.top(); pq.pop();
                while (v < n) {
                    v <<= 1;
                    v |= mn[v] > mn[v | 1];
                    pq.push(v ^ 1);
                }
                res[i] = v - n;
            }
            while (pq.size()) pq.pop();
            return res;
        }

        T operator[](size_t i) {assert(i < n); return mn[n + i];}
    };

    static constexpr T ZERO = 0;
    static constexpr T EPS = is_floating_point_v<T> ? 1e-14 : 0;
    static constexpr int INF = 1e9;

    using matrix_container =
    conditional_t<MatrixContainer == MAP, map<int, T, less<int>, Allocator<pair<const int, T>>>,
    conditional_t<MatrixContainer == UMAP, unordered_map<int, T, hash<int>, equal_to<int>, Allocator<pair<const int, T>>>,
    conditional_t<MatrixContainer == LINEAR_UMAP, linear_unordered_map<int, T, Allocator>,
    conditional_t<MatrixContainer == UMAP_SWITCHER, unordered_map_switcher<int, T, Allocator>,
    void>>>>;

    using column_container =
    conditional_t<ColumnContainer == SET, set<int, less<int>, Allocator<int>>,
    conditional_t<ColumnContainer == USET, unordered_set<int, hash<int>, equal_to<int>, Allocator<int>>,
    conditional_t<ColumnContainer == LINEAR_USET, linear_unordered_set<int, Allocator>,
    conditional_t<ColumnContainer == USET_SWITCHER, unordered_set_switcher<int, Allocator>,
    void>>>>;

    using Matrix = vecall(matrix_container);
    using Columns = vecall(column_container);

    size_t n;
    Columns y;
    Matrix u;
    vecall(T) b;

    __attribute__((always_inline))
    bool eq(T x, T y) { if constexpr (is_arithmetic_v<T>) return abs(x - y) <= EPS; else return x == y; }

    void _add(Columns& mp, Matrix& m, size_t i, size_t j, T x) {
        if (eq(x, ZERO)) return;
        if (m[i].count(j)) {
            m[i][j] += x;
            if (eq(m[i][j], ZERO)) m[i].erase(j), mp[j].erase(i);
        } else {
            m[i][j] = x;
            mp[j].insert(i);
        }
    }

public:
    sparse_gauss() = default;

    //n is the number of variables
    sparse_gauss(size_t n, size_t reserve_rows = 0, size_t reserve_cols = 8): n(n), y(n) {
        assert(n);
        if (reserve_rows) u.reserve(reserve_rows), b.reserve(reserve_rows);
        if constexpr (ColumnContainer != SET) {
            if (reserve_cols) for (auto& i : y) i.reserve(reserve_cols);
        }
    }

    void clear() {
        u.clear();
        for (auto& col : y) col.clear();
    }

    void add_equation(const vector<pair<size_t, T>>& coefs, T result) {
        u.emplace_back();
        if constexpr (MatrixContainer != MAP) u.back().reserve(coefs.size());
        for (auto [j, c] : coefs) {
            assert(j < n);
            _add(y, u, u.size() - 1, j, c);
        }
        b.push_back(result);
    }

    //Returns {# of solutions (2 if inf), one of solutions itself if exists}
    tuple<int, vector<T>> solve_SLA() {
        static const tuple<int, vector<T>> NO_SOLUTION = {0, {}, {}};
        size_t rows = u.size(), cols = n;
        assert(rows);
        segtree_point_upd srow(rows);
        segtree_point_upd scol(cols);
        for (size_t j = 0; j < cols; ++j) scol.mn[cols + j] = y[j].size();
        for (size_t i = 0; i < rows; ++i) srow.mn[rows + i] = u[i].size();
        for (size_t j = cols - 1; j; --j) scol.mn[j] = min(scol.mn[j << 1], scol.mn[j << 1 | 1]);
        for (size_t i = rows - 1; i; --i) srow.mn[i] = min(srow.mn[i << 1], srow.mn[i << 1 | 1]);
        auto normalize_row = [&](size_t i, size_t j) {
            assert(u[i][j] != ZERO);
            const T inv = T(1) / u[i][j];
            for (auto& [j, c] : u[i]) c *= inv;
            b[i] *= inv;
        };
        auto add_row = [&](size_t from, size_t to, T koef) {
            if (koef == ZERO) return;
            assert(to != from);
            int to_delta = 0;
            for (const auto& [j, c] : u[from]) {
                auto& t = u[to][j];
                if (eq(t, ZERO)) [[unlikely]] {
                    t += c * koef;
                    y[j].insert(to);
                    ++to_delta;
                    if constexpr (min_col) scol.point_add(j, 1);
                } else {
                    t += c * koef;
                    if (eq(t, ZERO)) {
                        u[to].erase(j), y[j].erase(to);
                        --to_delta;
                        if constexpr (min_col) scol.point_add(j, -1);
                    }
                }
            }
            srow.point_add(to, to_delta);
            b[to] += b[from] * koef;
        };
        vector<char> usr(rows), usc(cols);
        vector<int> rw(cols, -1);
        auto kill = [&](int i, int j) {
            if (i != -1) {
                usr[i] = 1;
                srow.point_add(i, INF);
            }
            if (j != -1) {
                usc[j] = 1;
                scol.point_add(j, INF);
            }
            if (i == -1 || j == -1) return;
            rw[j] = i;
            normalize_row(i, j);
            y[j].erase(i);
            while (y[j].size()) {
                int to = *y[j].begin();
                add_row(i, to, -u[to][j]);
            }
            y[j].insert(i);
        };
        auto find_best_i_by_j = [&](size_t j, size_t limit = -1) {
            int i = -1;
            for (size_t c = 0; int ii : y[j]) {
                if (!usr[ii]) {
                    if (c++ == 0 || u[ii].size() < u[i].size()) i = ii;
                    if (c > limit) break;
                }
            }
            return i;
        };
        auto find_best_j_by_i = [&](size_t i, size_t limit = -1) {
            int j = -1;
            for (size_t c = 0; const auto& [jj, val] : u[i]) {
                if (!usc[jj]) {
                    if (c++ == 0 || y[jj].size() < y[j].size()) j = jj;
                    if (c > limit) break;
                }
            }
            return j;
        };
        for (size_t nr = 0, nc = 0; nr < rows && nc < cols; ++nr, ++nc) {
            vector<pair<int, int>> cnds;
            if (min_col) {
                for (auto j : scol.pos_of_min_k(1)) {
                    if (usc[j]) break;
                    int i = find_best_i_by_j(j);
                    cnds.emplace_back(i, j);
                }
            } else {
                for (auto i : srow.pos_of_min_k(1)) {
                    if (usr[i]) break;
                    int j = find_best_j_by_i(i);
                    cnds.emplace_back(i, j);
                }
            }
            for (size_t _ = 0; _ < cnds.size(); ++_) {
                auto [i, j] = cnds[0];
                if (i == -1 || j == -1) break;
                auto [i1, j1] = cnds[_];
                if (i1 == -1 || j1 == -1 || u[i1].size() * y[j1].size() > u[i].size() * y[j].size()) {
                    if (_) swap(cnds[0], cnds[_]);
                }
            }
            auto [i, j] = cnds[0];
            if (i == -1) {--nr;}
            if (j == -1) {
                --nc;
                if (b[i] != ZERO) return NO_SOLUTION;
            }
            kill(i, j);
        }
        for (size_t i = 0; i < rows; ++i) {
            if (u[i].empty() && b[i] != ZERO) return NO_SOLUTION;
        }
        vector<T> solution(cols);
        size_t rk = cols;
        for (size_t i = 0; i < rows; ++i) {
            int j = -1;
            for (auto [jj, val] : u[i]) {
                if (rw[jj] != -1) {j = jj; break;}
            }
            if (j == -1) {
                if (u[i].size()) return NO_SOLUTION;
            } else {
                solution[j] = b[i];
                --rk;
            }
        }
        // Uncomment if you need basis and add it to returning tuple
        // vector basis(rk, vector<T>(cols));
        // for (size_t j = 0, t = 0; j < cols; ++j) {
        //     if (rw[j] != -1) continue;
        //     for (size_t jj = 0; jj < cols; ++jj) {
        //         if (rw[jj] != -1) basis[t][jj] = -u[rw[jj]][j];
        //     }
        //     basis[t++][j] = 1;
        // }
        return {1 + (rk > 0), solution};
    }
};
