namespace RomanNumeralSystem {
    const unordered_map<char, int> mp = {
        {'I', 1}, {'V', 5},
        {'X', 10}, {'L', 50},
        {'C', 100}, {'D', 500},
        {'M', 1000}
    };

    const string store[][10] = {
        {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"},
        {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"},
        {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"},
        {"", "M", "MM", "MMM", "?", "?", "?", "?", "?", "?"}
    };

    string int_to_roman(int n) {
        assert(1 <= n && n <= 3999);
        string ans;
        for (int i = 0; n; n /= 10, ++i) ans = store[i][n % 10] + ans;
        return ans;
    }

    int roman_to_int(string t) {
        assert(!t.empty());
        int o = mp.at(t.back());
        for (int q = 0; q + 1 < t.size(); q++) {
            o += (mp.at(t[q]) < mp.at(t[q + 1]) ? -1 : 1) * mp.at(t[q]);
        }
        return o;
    }
};