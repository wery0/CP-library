//Fast and high-precise (~ 10^-13) fft
//Replase ld by double = faster (25-35%), but less precision (~10^-10)

template<typename T>
struct Complex { T x = 0, y = 0; };
template<typename T>
T operator*(T l, T r) {
    return T{l.x * r.x - l.y * r.y, l.x * r.y + l.y * r.x};
}
template<typename T>
T& operator+=(T& l, T r) {
    l.x += r.x;
    l.y += r.y;
    return l;
}
template<typename T>
T operator-(T l, T r) {
    return T{l.x - r.x, l.y - r.y};
}

const double PI = 3.14159265358979323846264;
const int U = 1 << 16;
int perm[U * 2];

void build_perm(int l, int r, int s, int d) {
    if (l == r) {
        perm[l] = s;
        return;
    }
    const int md = (l + r) >> 1;
    build_perm(l, md, s, d << 1);
    build_perm(md + 1, r, s + d, d << 1);
}

Complex<ld> pw[U];
template<typename T>
void fft(int a, vec<Complex<T>> &m, int inv) {
    for (int q = 0; q < a; q++) {
        if (q < perm[q]) swap(m[q], m[perm[q]]);
    }
    for (int d = 2; d <= a; d <<= 1) {
        const int md = d >> 1;
        pw[0] = {1, 0};
        pw[1] = {cosl(2 * PI / d), (inv ? -1 : 1) * sinl(2 * PI / d)};
        for (int q = 2; q < md; q++) pw[q] = pw[q - 1] * pw[1];
        for (int l = 0; l < a; l += md) {
            for (int q = 0; q < md; q++, l++) {
                const Complex<T> p = Complex<T>({(T)pw[q].x, (T)pw[q].y}) * m[l + md];
                m[l + md] = m[l] - p;
                m[l] += p;
            }
        }
    }
}

template<typename T>
vec<T> mul(vec<T> _m, vec<T> _n) {
    int res_size = _m.size() + _n.size() - 1;
    int a = geq_pow2(max(_m.size(), _n.size()));
    vec<Complex<ld>> m(a * 2), n(a * 2);
    for (int q = 0; q < _m.size(); q++) m[q].x = _m[q];
    for (int q = 0; q < _n.size(); q++) n[q].x = _n[q];
    build_perm(0, a * 2 - 1, 0, 1);
    fft(a * 2, m, 0);
    fft(a * 2, n, 0);
    for (int q = 0; q < a * 2; q++) m[q] = m[q] * n[q];
    fft(a * 2, m, 1);
    vec<T> ans(res_size);
    for (int q = 0; q < res_size; ++q) {
        ans[q] = round(m[q].x / (a * 2));
    }
    return ans;
}