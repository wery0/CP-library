/*
Encodes bases[i]-based sequences a[i] into a single b-based sequence
Encode size: at most min(bits + d - 1, sum(|a[i]|)) where bits is theoretically smallest amount of needed b-based elements, i. e. bits = ceil(sum(|a[i]| * log(bases[i])) / log(b))
Complexity: O((bits + d) * sum(|a[i]|) / d)
Constraints: b >= 2, d >= 1, b * max(bases) must fit into T
Notes:
  - When d = k instead of d = 1, encoding works k times faster and produces encoding longer by at most k - 1 elements
  - If we raise all bases to power k (and correspondingly change the input sequences), encoding will be about k times faster
  - If we raise b to power k, encoding will be about k times faster
  - Both in encode() and decode() functions you can use faster methods of finding quotient and remainder in innermost loop since divisors are known in advance
*/
template<typename T>
vector<T> encode(T b, vector<T> bases, vector<vector<T>> a, T d = 1) {
    static_assert(is_integral_v<T>);
    if (a.empty()) return {};
    assert(b >= 2);
    assert(d >= 1);
    assert(bases.size() == a.size());
    assert(numeric_limits<T>::max() / b > *max_element(bases.begin(), bases.end()));
    int n = bases.size();
    vector<T> code;
    vector<vector<T>> na(n);
    auto go = [&]() {
        long double sm = 0;
        for (int i = 0; i < n; ++i) sm += na[i].size() * logl(bases[i]);
        int bits = ceill(sm / logl(b));
        code.reserve(code.size() + bits);
        for (int p = 0, sti = n - 1, stj = na.back().size(); p < bits; ++p) {
            for (; sti >= 0; --sti) {
                for (; stj; --stj) if (na[sti][stj - 1]) break;
                if (stj) break;
                stj = sti ? na[sti - 1].size() : 0;
            }
            T rem = 0;
            for (int i = sti; i >= 0; --i) {
                for (int j = int(i == sti ? stj : na[i].size()) - 1; j >= 0; --j) {
                    rem = rem * bases[i] + na[i][j];
                    na[i][j] = rem / b;
                    rem %= b;
                }
            }
            code.push_back(rem);
        }
    };
    for (int i = 0; i < d; ++i) {
        for (int j = 0; j < n; ++j) {
            size_t l = a[j].size() * i / d;
            size_t r = a[j].size() * (i + 1) / d;
            na[j] = vector<T>(a[j].begin() + l, a[j].begin() + r);
        }
        go();
    }
    return code;
}

//Decodes b-based sequence code into |lens| bases[i]-based sequences of lengths lens[i]
//Complexity: O(|code| * sum(lens) / d)
template<typename T>
vector<vector<T>> decode(T b, vector<T> bases, vector<T> lens, vector<T> code, T d = 1) {
    static_assert(is_integral_v<T>);
    assert(b > 1);
    assert(bases.size() == lens.size());
    int n = bases.size();
    vector<vector<T>> a(n);
    for (int i = 0; i < n; ++i) a[i].resize(lens[i]);
    vector<int> ptr(n);
    auto go = [&](vector<T> lens, vector<T> code) {
        int m = code.size();
        for (int i = 0, stp = m - 1; i < n; ++i) {
            for (int j = 0; j < lens[i]; ++j) {
                while (stp >= 0 && code[stp] == 0) --stp;
                T rem = 0;
                for (int p = stp; p >= 0; --p) {
                    rem = rem * b + code[p];
                    code[p] = rem / bases[i];
                    rem %= bases[i];
                }
                a[i][ptr[i] + j] = rem;
            }
        }
    };
    for (int i = 0, u = 0; i < d; ++i) {
        vector<T> nl(n);
        long double sm = 0;
        for (int j = 0; j < n; ++j) {
            ptr[j] = lens[j] * i / d;
            nl[j] = lens[j] * (i + 1) / d - ptr[j];
            sm += nl[j] * logl(bases[j]);
        }
        int bits = ceill(sm / logl(b));
        go(nl, vector<T>(code.begin() + u, code.begin() + u + bits));
        u += bits;
    }
    return a;
}
