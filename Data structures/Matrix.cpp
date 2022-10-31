template<typename T>
struct Matrix {
    int a = 0, b = 0;
    vec<vec<T>> m;

    Matrix() = default;
    Matrix(int _a, int _b): a(_a), b(_b) {m = vec<vec<T>>(a, vec<T>(b));}
    Matrix(const Matrix<T> &o) {a = o.a, b = o.b, m = o.m;}
    template<typename U>
    Matrix(const vec<vec<U>> &o) {
        a = o.size(), b = a ? o[0].size() : 0;
        m = vec<vec<T>>(a, vec<T>(b));
        for (int q = 0; q < a; ++q) for (int w = 0; w < b; ++w) m[q][w] = o[q][w];
    }
    template<typename U>
    Matrix(const initializer_list<initializer_list<U>> &l) {
        a = l.size(), b = a ? (*l.begin()).size() : 0;
        m = vec<vec<T>>(a, vec<T>(b));
        int q = 0;
        for (const auto &i : l) {
            assert(b == i.size());
            int w = 0;
            for (const auto &j : i) m[q][w++] = j;
            ++q;
        }
    }

    Matrix& operator=(const Matrix<T> &o) {a = o.a, b = o.b, m = o.m; return *this;}
    Matrix& operator=(ll x) {for (int q = 0; q < min(a, b); ++q) m[q][q] = x; return *this;}

    void fill(T x) {for (auto &i : m) std::fill(all(i), x);}

    inline bool is_sum_compatible(const Matrix& o) const {return a == o.a && b == o.b;}
    inline bool is_mul_compatible(const Matrix& o) const {return b == o.a;}

    Matrix operator+(const Matrix<T> &o) const {
        assert(is_sum_compatible(o));
        Matrix<T> res = *this;
        for (int q = 0; q < a; ++q) for (int w = 0; w < b; ++w) res[q][w] += o[q][w];
        return res;
    }
    Matrix operator-(const Matrix<T> &o) const {
        assert(is_sum_compatible(o));
        Matrix<T> res = *this;
        for (int q = 0; q < a; ++q) for (int w = 0; w < b; ++w) res[q][w] -= o[q][w];
        return res;
    }
    Matrix operator*(const Matrix<T> &o) const {
        assert(is_mul_compatible(o));
        int c = o.b;
        Matrix<T> res(a, c);
        Matrix<T> tro = o;
        tro.self_transpose();
        for (int q = 0; q < a; ++q) {
            for (int w = 0; w < c; ++w) {
#pragma GCC unroll 16
                for (int e = 0; e < b; ++e) res[q][w] += m[q][e] * tro[w][e];
            }
        }
        return res;
    }

    Matrix get_pow(ll k) {
        assert(a == b);
        Matrix<T> o(a, a), x = *this;
        for (o = 1; k; k >>= 1) {
            if (k & 1) o *= x;
            x *= x;
        }
        return o;
    }
    void self_binpow(ll k) {
        assert(a == b);
        Matrix<T> o(a, a), x = *this;
        for (o = 1; k; k >>= 1) {
            if (k & 1) o *= x;
            x *= x;
        }
        *this = o;
    }

    void self_transpose() {vec<vec<T>> n(b, vec<T>(a)); for (int w = 0; w < b; ++w) for (int q = 0; q < a; ++q) n[w][q] = m[q][w]; m = n; swap(a, b);}
    Matrix get_transpose() {vec<vec<T>> n(b, vec<T>(a)); for (int w = 0; w < b; ++w) for (int q = 0; q < a; ++q) n[w][q] = m[q][w]; return Matrix(n);}

    void operator+=(const Matrix<T> &o) {assert(is_sum_compatible(o)); for (int q = 0; q < a; ++q) for (int w = 0; w < b; ++w) m[q][w] += o[q][w];}
    void operator-=(const Matrix<T> &o) {assert(is_sum_compatible(o)); for (int q = 0; q < a; ++q) for (int w = 0; w < b; ++w) m[q][w] += o[q][w];}
    void operator*=(const Matrix<T> &o) {(*this) = (*this) * o;}

    const vec<T>& operator[](uint x) const {assert(0 <= x && x < a); return m[x];}
    vec<T>& operator[](uint x) {assert(0 <= x && x < a); return m[x];}

    friend istream& operator>>(istream& in, Matrix& m) {for (int q = 0; q < m.a; ++q) for (int w = 0; w < m.b; ++w) in >> m[q][w]; return in;}
    friend ostream& operator<<(ostream& out, const Matrix& m) {for (int q = 0; q < m.a; ++q) {for (int w = 0; w < m.b; ++w) {out << m[q][w]; if (w + 1 != m.b) out << ' ';} out << '\n';} return out;}
};