__int128_t abs(__int128_t x) {return x < 0 ? -x : x;}
__int128_t stoi128(string s) {int sign = s[0] == '-' ? -1 : 1; __int128_t x = 0; for (size_t i = sign == -1; i < s.size(); ++i) x = x * 10 + s[i] - '0'; return x * sign;}
string to_string(__int128_t x) {int sign = x < 0 ? -1 : 1; x *= sign; if (x == 0) return "0"; string s; while (x) s += '0' + x % 10, x /= 10; if (sign == -1) s += '-'; reverse(s.begin(), s.end()); return s;}
string to_string(__uint128_t x) {if (x == 0) return "0"; string s; while (x) s += '0' + x % 10, x /= 10; reverse(s.begin(), s.end()); return s;}
istream& operator>>(istream& in, __int128_t& x) {string s; in >> s; x = stoi128(s); return in;}
ostream& operator<<(ostream& out, __int128_t x) {return out << to_string(x);}
istream& operator>>(istream& in, __uint128_t& x) {string s; in >> s; x = 0; for (char c : s) x = x * 10 + c - '0'; return in;}
ostream& operator<<(ostream& out, __uint128_t x) {return out << to_string(x);}

//stupid ones
istream& operator>>(istream& in, __float128& x) {long double t; in >> t; x = (__float128)(t); return in;}
ostream& operator<<(ostream& out, __float128 x) {return out << (long double) (x);}
