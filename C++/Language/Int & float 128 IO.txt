istream& operator>>(istream &in, __int128_t &x) {str t; in >> t; int sign = t[0] == '-' ? -1 : 1; x = 0; for (int q = sign == -1; q < t.size(); ++q) {x = x * 10 + t[q] - '0';} x *= sign; return in;}
ostream& operator<<(ostream &out, __int128_t x) {int sign = x < 0 ? -1 : 1; x *= sign; if (x == 0) return out << "0"; str t; for (; x; x /= 10) t.pb('0' + x % 10); if (sign == -1) t.pb('-'); reverse(all(t)); return out << t;}
istream& operator>>(istream &in, __uint128_t &x) {str t; in >> t; x = 0; for (char c : t) x = x * 10 + c - '0'; return in;}
ostream& operator<<(ostream &out, __uint128_t x) {if (x == 0) return out << "0"; str t; for (; x; x /= 10) t.pb('0' + x % 10); reverse(all(t)); return out << t;}

//stupid ones
istream& operator>>(istream &in, __float128 &x) {long double t; in >> t; x = (__float128)(t); return in;}
ostream& operator<<(ostream &out, __float128 x) {return out << (long double)(x);}