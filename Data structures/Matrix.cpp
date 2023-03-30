template<typename T>
class Matrix {
    size_t a = 0, b = 0;
    vector<vector<T>> m;

public:
    Matrix() = default;
    Matrix(size_t n, size_t m): a(n), b(m), m(a, vector<T>(b)) {}
    Matrix(const Matrix<T>& rhs): a(rhs.a), b(rhs.b), m(rhs.m) {}

    template<typename U>
    Matrix(const vector<vector<U>>& o): a(o.size()), b(a ? o[0].size() : 0), m(a) {
        for (size_t i = 0; i < a; ++i) m[i] = vector<T>(o[i].begin(), o[i].end());
    }

    template<typename U>
    Matrix(const initializer_list<initializer_list<U>>& l): a(l.size()), b(a ? (*l.begin()).size() : 0), m(a) {
        for (size_t i = 0; const auto& c : l) {
            assert(b == c.size());
            m[i++] = vector<T>(c.begin(), c.end());
        }
    }

    Matrix& operator=(const Matrix<T>& rhs) {a = rhs.a, b = rhs.b, m = rhs.m; return *this;}
    Matrix& operator=(T val) {for (size_t i = 0; i < min(a, b); ++i) m[i][i] = val; return *this;}

    Matrix operator+(const Matrix<T>& rhs) const {assert(is_sum_compatible(rhs)); Matrix<T> res = *this; for (size_t i = 0; i < a; ++i) for (size_t j = 0; j < b; ++j) res[i][j] += rhs[i][j]; return res;}
    Matrix operator-(const Matrix<T>& rhs) const {assert(is_sum_compatible(rhs)); Matrix<T> res = *this; for (size_t i = 0; i < a; ++i) for (size_t j = 0; j < b; ++j) res[i][j] -= rhs[i][j]; return res;}
    Matrix operator*(const Matrix<T>& rhs) const {
        assert(is_mul_compatible(rhs));
        const size_t c = rhs.b;
        Matrix<T> res(a, c), tr = rhs.get_transpose();
        for (size_t i = 0; i < a; ++i) {
            for (size_t j = 0; j < c; ++j) {
#pragma GCC unroll 16
                for (size_t k = 0; k < b; ++k) res[i][j] += m[i][k] * tr[j][k];
            }
        }
        return res;
    }
    Matrix& operator+=(const Matrix<T>& rhs) {assert(is_sum_compatible(rhs)); for (size_t i = 0; i < a; ++i) for (size_t j = 0; j < b; ++j) m[i][j] += rhs[i][j]; return *this;}
    Matrix& operator-=(const Matrix<T>& rhs) {assert(is_sum_compatible(rhs)); for (size_t i = 0; i < a; ++i) for (size_t j = 0; j < b; ++j) m[i][j] -= rhs[i][j]; return *this;}
    Matrix& operator*=(const Matrix<T>& rhs) {return *this = *this * rhs;}

    vector<T>& operator[](size_t i) {assert(0 <= i && i < a); return m[i];}
    const vector<T>& operator[](size_t i) const {assert(0 <= i && i < a); return m[i];}

    size_t size() const {return a;}
    void fill(T val) {for (auto& row : m) fill(row.begin(), row.end(), val);}

    bool is_sum_compatible(const Matrix& rhs) const {return a == rhs.a && b == rhs.b;}
    bool is_mul_compatible(const Matrix& rhs) const {return b == rhs.a;}

    Matrix get_pow(long long k) const {assert(a == b); assert(k >= 0); Matrix<T> res(a, a), x = *this; for (res = 1; k; k >>= 1, x *= x) if (k & 1) res *= x; return res;}
    void self_pow(long long k) {*this = get_pow(k);}

    //For these two functions, T must be invertible
    T calc_determinant() const {assert(a == b); T res = 1; auto n = m; for (size_t i = 0; i < a; ++i) {size_t ii = i; while (ii < a && n[ii][i] == 0) ++ii; if (ii == a) return 0; if (ii != i) swap(n[i], n[ii]), res *= T(-1); res *= n[i][i]; T inv = T(1) / n[i][i]; for (size_t j = i; j < a; ++j) n[i][j] *= inv; for (size_t ii = i + 1; ii < a; ++ii) {const T koef = n[ii][i]; if (koef == 0) continue; for (size_t j = i; j < a; ++j) n[ii][j] -= n[i][j] * koef;}} return res;}
    Matrix calc_inverse() const {assert(a == b); auto n = m; for (size_t i = 0; i < a; ++i) {n[i].resize(a * 2); n[i][a + i] = 1;} for (size_t i = 0; i < a; ++i) {size_t ii = i; while (ii < a && n[ii][i] == 0) ++ii; assert(ii < a && "Inverse matrix doesn't exist"); if (ii != i) swap(n[i], n[ii]); T inv = T(1) / n[i][i]; for (size_t j = i; j < a * 2; ++j) n[i][j] *= inv; for (size_t ii = i + 1; ii < a; ++ii) {const T koef = n[ii][i]; if (koef == 0) continue; for (size_t j = i; j < a * 2; ++j) n[ii][j] -= n[i][j] * koef;}} for (size_t q = 1; q < a; ++q) {for (size_t i = 0; i < q; ++i) {const T koef = n[i][q]; if (koef == 0) continue; for (size_t j = q; j < a * 2; ++j) {n[i][j] -= koef * n[q][j];}}} for (size_t i = 0; i < a; ++i) {rotate(n[i].begin(), n[i].begin() + a, n[i].end()); n[i].resize(a);} return n;}

    Matrix get_transpose() const {vector<vector<T>> n(b, vector<T>(a)); for (size_t j = 0; j < b; ++j) for (size_t i = 0; i < a; ++i) n[j][i] = m[i][j]; return Matrix(n);}
    void self_transpose() {*this = get_transpose();}

    friend istream& operator>>(istream& in, Matrix& m) {for (size_t i = 0; i < m.a; ++i) for (size_t j = 0; j < m.b; ++j) in >> m[i][j]; return in;}
    friend ostream& operator<<(ostream& out, const Matrix& m) {for (size_t i = 0; i < m.a; ++i) for (size_t j = 0; j < m.b; ++j) out << m[i][j] << " \n"[j == m.b - 1]; return out;}
};