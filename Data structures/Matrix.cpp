template<typename T>
class Matrix {
    size_t n = 0, m = 0;
    vector<vector<T>> t;

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

    //For these two functions, T must be invertible
    T calc_determinant() const {assert(n == m); T res = 1; auto u = t; for (size_t i = 0; i < n; ++i) {size_t ii = i; while (ii < n && u[ii][i] == (T)0) ++ii; if (ii == n) return 0; if (ii != i) swap(u[i], u[ii]), res *= T(-1); res *= u[i][i]; T inv = T(1) / u[i][i]; for (size_t j = i; j < n; ++j) u[i][j] *= inv; for (size_t ii = i + 1; ii < n; ++ii) {const T koef = u[ii][i]; if (koef == (T)0) continue; for (size_t j = i; j < n; ++j) u[ii][j] -= u[i][j] * koef;}} return res;}
    Matrix calc_inverse() const {assert(n == m); auto u = t; for (size_t i = 0; i < n; ++i) {u[i].resize(n * 2); u[i][n + i] = 1;} for (size_t i = 0; i < n; ++i) {size_t ii = i; while (ii < n && u[ii][i] == (T)0) ++ii; if (ii >= n) return {}; assert(ii < n && "Inverse matrix doesn't exist"); if (ii != i) swap(u[i], u[ii]); T inv = T(1) / u[i][i]; for (size_t j = i; j < n * 2; ++j) u[i][j] *= inv; for (size_t ii = i + 1; ii < n; ++ii) {const T koef = u[ii][i]; if (koef == (T)0) continue; for (size_t j = i; j < n * 2; ++j) u[ii][j] -= u[i][j] * koef;}} for (size_t q = 1; q < n; ++q) {for (size_t i = 0; i < q; ++i) {const T koef = u[i][q]; if (koef == (T)0) continue; for (size_t j = q; j < n * 2; ++j) {u[i][j] -= koef * u[q][j];}}} for (size_t i = 0; i < n; ++i) {rotate(u[i].begin(), u[i].begin() + n, u[i].end()); u[i].resize(n);} return u;}

    Matrix get_transpose() const {vector<vector<T>> u(m, vector<T>(n)); for (size_t j = 0; j < m; ++j) for (size_t i = 0; i < n; ++i) u[j][i] = t[i][j]; return Matrix(u);}
    void self_transpose() {*this = get_transpose();}

    friend istream& operator>>(istream& in, Matrix& rhs) {for (size_t i = 0; i < rhs.n; ++i) for (size_t j = 0; j < rhs.m; ++j) in >> rhs[i][j]; return in;}
    friend ostream& operator<<(ostream& out, const Matrix& rhs) {for (size_t i = 0; i < rhs.n; ++i) for (size_t j = 0; j < rhs.m; ++j) out << rhs[i][j] << " \n"[j == rhs.m - 1]; return out;}
};