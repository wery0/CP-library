void fft(vector<complex<double>>& a) {
    int n = a.size(), L = __lg(n);
    static vector<complex<long double>> R(2, 1);
    static vector<complex<double>> rt(2, 1);  // (^ 10% faster if double)
    for (static int k = 2; k < n; k *= 2) {
        R.resize(n); rt.resize(n);
        auto x = polar(1.0L, acos(-1.0L) / k);
        for (int i = k; i < k * 2; ++i) rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
    }
    vector<int> rev(n);
    for (int i = 0; i < n; ++i) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    for (int i = 0; i < n; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += k * 2) {
            for (int j = 0; j < k; ++j) {
                // complex<double> z = rt[j+k] * a[i+j+k]; // (25% faster if hand-rolled)  /// include-line
                auto x = (double*)&rt[j + k], y = (double*)&a[i + j + k];                  /// exclude-line
                complex<double> z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);   /// exclude-line
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
        }
    }
}

vector<double> sum_convolution_real(const vector<double>& a, const vector<double>& b) {
    if (a.empty() || b.empty()) return {};
    vector<double> res(a.size() + b.size() - 1);
    int L = __lg(res.size()) + 1, n = 1 << L;
    vector<complex<double>> in(n), out(n);
    copy(a.begin(), a.end(), in.begin());
    for (int i = 0; i < b.size(); ++i) in[i].imag(b[i]);
    fft(in);
    for (complex<double>& x : in) x *= x;
    for (int i = 0; i < n; ++i) out[i] = in[-i & (n - 1)] - conj(in[i]);
    fft(out);
    for (int i = 0; i < res.size(); ++i) res[i] = imag(out[i]) / (4 * n);
    return res;
}

template<const int32_t mod>
vector<int64_t> sum_convolution_real_arbitrary_mod(const vector<int64_t>& a, const vector<int64_t>& b) {
    if (a.empty() || b.empty()) return {};
    vector<int64_t> res(a.size() + b.size() - 1);
    int B = __lg(res.size()) + 1, n = 1 << B, cut = int(sqrt(mod));
    vector<complex<double>> L(n), R(n), outs(n), outl(n);
    for (int i = 0; i < a.size(); ++i) L[i] = complex<double>(int(a[i]) / cut, int(a[i]) % cut);
    for (int i = 0; i < b.size(); ++i) R[i] = complex<double>(int(b[i]) / cut, int(b[i]) % cut);
    fft(L), fft(R);
    for (int i = 0; i < n; ++i) {
        int j = -i & (n - 1);
        outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
        outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / 1i;
    }
    fft(outl), fft(outs);
    for (int i = 0; i < res.size(); ++i) {
        int64_t av = int64_t(real(outl[i]) + .5), cv = int64_t(imag(outs[i]) + .5);
        int64_t bv = int64_t(imag(outl[i]) + .5) + int64_t(real(outs[i]) + .5);
        res[i] = ((av % mod * cut + bv) % mod * cut + cv) % mod;
    }
    return res;
}
