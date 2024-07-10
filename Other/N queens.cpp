//Places n queens on n * n board s. t. no two attack each other
//Returns vector y of length n, s. t. {(i, y[i]) | 0 <= i < n} is correct queen placement
//O(n)
vector<int> n_queens(const size_t n) {
    if (n == 1) return {0};
    if (n < 4) return {};
    const size_t rem = n % 12;
    vector<int> y;
    for (int i = 1; i < n; i += 2) y.push_back(i);
    if (rem == 3 || rem == 9) y.erase(y.begin()), y.push_back(1);
    if (rem == 8) {
        for (int i = 2, j = 0; j < n / 2; i += j & 1 ? 6 : -2, ++j) y.push_back(i);
    } else {
        for (int i = 0; i < n; i += 2) y.push_back(i);
    }
    if (rem == 2 || rem == 3 || rem == 9) {
        vector<int> ny;
        if (rem == 2) {
            swap(y[n / 2], y[n / 2 + 1]);
            if (n >= 5) {
                for (int i = 0; i < n; ++i) {
                    if (y[i] != 4) ny.push_back(y[i]);
                }
                ny.push_back(4);
            }
        } else {
            for (int i = 0; i < n; ++i) {
                if (y[i] != 0 && y[i] != 2) ny.push_back(y[i]);
            }
            ny.push_back(0);
            ny.push_back(2);
        }
        y = ny;
    }
    return y;
}
