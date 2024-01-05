template<typename T>
class Matrix {
    static constexpr T ZERO = 0;

    size_t n = 0, m = 0;
    vector<vector<T>> t;

    static void add_row(vector<T>& from, vector<T>& to, size_t sti, T koef) {
        if (koef == ZERO) return;
        const size_t N = to.size();
        assert(from.size() == N);
        for (size_t i = sti; i < N; ++i) to[i] += from[i] * koef;
    }

    static void normalize_row(vector<T>& row, size_t i) {
        const size_t N = row.size();
        assert(i < N);
        assert(row[i] != ZERO);
        const T inv = T(1) / row[i];
        for (size_t j = i; j < N; ++j) row[j] *= inv;
    }

    static vector<vector<T>> transpose_vv(const vector<vector<T>>& t) {
        const size_t n = t.size();
        const size_t m = n ? t[0].size() : 0;
        vector<vector<T>> u(m, vector<T>(n));
        for (size_t j = 0; j < m; ++j) for (size_t i = 0; i < n; ++i) u[j][i] = t[i][j];
        return u;
    }

public:
    Matrix() = default;
    Matrix(size_t n, size_t m): n(n), m(m), t(n, vector<T>(m)) {}
    Matrix(const Matrix<T>& rhs): n(rhs.n), m(rhs.m), t(rhs.t) {}

    template<typename U>
    Matrix(const vector<vector<U>>& rhs): n(rhs.size()), m(n ? rhs[0].size() : 0), t(n) {
        for (size_t i = 0; i < n; ++i) t[i] = vector<T>(rhs[i].begin(), rhs[i].end());
    }

    template<typename U>
    Matrix(const initializer_list<initializer_list<U>>& l): n(l.size()), m(n ? (*l.begin()).size() : 0), t(n) {
        for (size_t i = 0; const auto& c : l) {
            assert(m == c.size());
            t[i++] = vector<T>(c.begin(), c.end());
        }
    }

    Matrix& operator=(const Matrix<T>& rhs) {n = rhs.n, m = rhs.m, t = rhs.t; return *this;}
    Matrix& operator=(T val) {fill(0); for (size_t i = 0; i < min(n, m); ++i) t[i][i] = val; return *this;}

    Matrix operator+(const Matrix<T>& rhs) const {assert(is_sum_compatible(rhs)); Matrix<T> res = *this; for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < m; ++j) res[i][j] += rhs[i][j]; return res;}
    Matrix operator-(const Matrix<T>& rhs) const {assert(is_sum_compatible(rhs)); Matrix<T> res = *this; for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < m; ++j) res[i][j] -= rhs[i][j]; return res;}
    Matrix operator*(const Matrix<T>& rhs) const {
        assert(is_mul_compatible(rhs));
        const size_t c = rhs.m;
        Matrix<T> res(n, c), tr = rhs.get_transpose();
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < c; ++j) {
#pragma GCC unroll 16
                for (size_t k = 0; k < m; ++k) res[i][j] += t[i][k] * tr[j][k];
            }
        }
        return res;
    }
    Matrix& operator+=(const Matrix<T>& rhs) {assert(is_sum_compatible(rhs)); for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < m; ++j) t[i][j] += rhs[i][j]; return *this;}
    Matrix& operator-=(const Matrix<T>& rhs) {assert(is_sum_compatible(rhs)); for (size_t i = 0; i < n; ++i) for (size_t j = 0; j < m; ++j) t[i][j] -= rhs[i][j]; return *this;}
    Matrix& operator*=(const Matrix<T>& rhs) {return *this = *this * rhs;}

    vector<T>& operator[](size_t i) {assert(0 <= i && i < n); return t[i];}
    const vector<T>& operator[](size_t i) const {assert(0 <= i && i < n); return t[i];}

    size_t size() const {return n;}
    bool empty() const {return n == 0;}
    void fill(T val) {for (auto& row : t) std::fill(row.begin(), row.end(), val);}

    bool is_sum_compatible(const Matrix& rhs) const {return n == rhs.n && m == rhs.m;}
    bool is_mul_compatible(const Matrix& rhs) const {return m == rhs.n;}

    Matrix get_pow(int64_t k) const {assert(n == m); assert(k >= 0); Matrix<T> res(n, n), x = *this; for (res = 1; k; k >>= 1, x *= x) if (k & 1) res *= x; return res;}
    void self_pow(int64_t k) {*this = get_pow(k);}

    //For these three functions, T must be invertible
    //O(min(n, m) ^ 2 * max(n, m))
    size_t calc_rank() const {auto u = t; if (n > m) u = transpose_vv(u); const size_t n = u.size(); const size_t m = n ? u[0].size() : 0; size_t rk = 0; for (size_t i = 0, c = 0; i < n && c < m; ++i, ++c, ++rk) {size_t r; for (; c < m; ++c) {r = i; while (r < n && u[r][c] == ZERO) ++r; if (r != n) break;} if (c == m) break; if (r != i) swap(u[i], u[r]); normalize_row(u[i], c); for (size_t j = r + 1; j < n; ++j) {add_row(u[i], u[j], c, -u[j][c]);}} return rk;}
    T calc_determinant() const {assert(n == m); T res = 1; auto u = t; for (size_t i = 0; i < n; ++i) {size_t r = i; while (r < n && u[r][i] == ZERO) ++r; if (r == n) return 0; if (r != i) swap(u[i], u[r]), res *= T(-1); res *= u[i][i]; normalize_row(u[i], i); for (size_t j = r + 1; j < n; ++j) {add_row(u[i], u[j], i, -u[j][i]);}} return res;}
    Matrix calc_inverse() const {assert(n == m); auto u = t; for (size_t i = 0; i < n; ++i) {u[i].resize(n * 2); u[i][n + i] = 1;} for (size_t i = 0; i < n; ++i) {size_t r = i; while (r < n && u[r][i] == ZERO) ++r; if (r >= n) return {}; assert(r < n && "Inverse matrix doesn't exist"); if (r != i) swap(u[i], u[r]); normalize_row(u[i], i); for (size_t j = r + 1; j < n; ++j) {add_row(u[i], u[j], i, -u[j][i]);}} for (size_t i = 1; i < n; ++i) {for (size_t j = 0; j < i; ++j) {add_row(u[i], u[j], i, -u[j][i]);}} for (size_t i = 0; i < n; ++i) {rotate(u[i].begin(), u[i].begin() + n, u[i].end()); u[i].resize(n);} return u;}

    Matrix get_transpose() const {return Matrix(transpose_vv(t));}
    void self_transpose() {*this = get_transpose();}

    friend istream& operator>>(istream& in, Matrix& rhs) {for (size_t i = 0; i < rhs.n; ++i) for (size_t j = 0; j < rhs.m; ++j) in >> rhs[i][j]; return in;}
    friend ostream& operator<<(ostream& out, const Matrix& rhs) {for (size_t i = 0; i < rhs.n; ++i) for (size_t j = 0; j < rhs.m; ++j) out << rhs[i][j] << " \n"[j == rhs.m - 1]; return out;}
};
