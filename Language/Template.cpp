#pragma GCC optimize("Ofast")
// #pragma GCC target("avx,avx2,fma")

#include "bits/stdc++.h"

//#define NDEBUG
#define F first
#define S second
#define vec vector
#define pb push_back
#define pll pair<ll, ll>
#define pdd pair<ld, ld>
#define pii pair<int, int>
#define pnn pair<Node*, Node*>
#define all(m) m.begin(), m.end()
#define rall(m) m.rbegin(), m.rend()
#define uid uniform_int_distribution
#define timeStamp() std::chrono::steady_clock::now()
#define unify(m) sort(all(m)); m.erase(unique(all(m)), m.end());
#define duration_micro(a) chrono::duration_cast<chrono::microseconds>(a).count()
#define duration_milli(a) chrono::duration_cast<chrono::milliseconds>(a).count()
#define fast cin.tie(0); cout.tie(0); cin.sync_with_stdio(0); cout.sync_with_stdio(0);
using namespace std;
using str = string;
using ll = long long;
using ld = long double;
using uint = unsigned int;
using ull = unsigned long long;
mt19937 rnd(timeStamp().time_since_epoch().count());
mt19937_64 rndll(timeStamp().time_since_epoch().count());
template<typename T> constexpr inline int sign(T x) {return x < 0 ? -1 : x > 0 ? 1 : 0;}
template<typename T, typename U> bool chmin(T& a, const U& b) {return (T)b < a ? a = b, 1 : 0;}
template<typename T, typename U> bool chmax(T& a, const U& b) {return (T)b > a ? a = b, 1 : 0;}
constexpr inline uint leq_pow2(const uint x) {return 1u << __lg(x);}
constexpr inline ull leq_pow2ll(const ull x) {return 1ull << __lg(x);}
constexpr inline uint geq_pow2(const uint x) {return x & (x - 1) ? 2u << __lg(x) : x;}
constexpr inline ull geq_pow2ll(const ull x) {return x & (x - 1) ? 2ull << __lg(x) : x;}
constexpr inline ll sqd(const pll p1, const pll p2) {return (p1.F - p2.F) * (p1.F - p2.F) + (p1.S - p2.S) * (p1.S - p2.S);}
constexpr inline ll sqd(const ll x1, const ll y1, const ll x2, const ll y2) {return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);}
struct custom_hash {static uint64_t xs(uint64_t x) {x += 0x9e3779b97f4a7c15; x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9; x = (x ^ (x >> 27)) * 0x94d049bb133111eb; return x ^ (x >> 31);} template<typename T> size_t operator()(T x) const {static const uint64_t C = timeStamp().time_since_epoch().count(); return xs(hash<T> {}(x) + C);}};
template<typename K> using uset = unordered_set<K, custom_hash>;
template<typename K, typename V> using umap = unordered_map<K, V, custom_hash>;
template<typename T1, typename T2> ostream& operator<<(ostream& out, const pair<T1, T2>& x) {return out << x.F << ' ' << x.S;}
template<typename T1, typename T2> istream& operator>>(istream& in, pair<T1, T2>& x) {return in >> x.F >> x.S;}
template<typename T, size_t N> istream& operator>>(istream& in, array<T, N>& a) {for (auto &x : a) in >> x; return in;};
template<typename T, size_t N> ostream& operator<<(ostream& out, const array<T, N>& a) {for (int q = 0; q < a.size(); ++q) {out << a[q];if (q + 1 < a.size()) out << ' ';}return out;};
template<typename T> istream& operator>>(istream& in, vector<T>& a) {for (auto &x : a) in >> x; return in;};
template<typename T> ostream& operator<<(ostream& out, const vector<T>& a) {for (int q = 0; q < a.size(); ++q) {out << a[q]; if (q + 1 < a.size()) out << ' ';} return out;};

int main() {
    fast;
    
}