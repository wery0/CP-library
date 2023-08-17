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
        string res;
        for (size_t i = 0; n; n /= 10, ++i) res = store[i][n % 10] + res;
        return res;
    }

    int roman_to_int(const string& s) {
        assert(!s.empty());
        int res = mp.at(s.back());
        for (size_t i = 0; i + 1 < s.size(); ++i) {
            res += (mp.at(s[i]) < mp.at(s[i + 1]) ? -1 : 1) * mp.at(s[i]);
        }
        return res;
    }
};
