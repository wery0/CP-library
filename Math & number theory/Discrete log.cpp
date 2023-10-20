//Solves equation a ^ x = b (mod p), p is prime
//If all_solutions == false, returns the smallest if it exists
//If all_solutions == true, returns all solutions in range [0; p)
//O(sqrt(p))
template<typename T>
vector<T> discrete_log(T a, T b, T p, bool all_solutions) {
    auto binpow = [&](T x, T k) {  //Be careful with overflow
        T o = 1;
        for (; k; k >>= 1) {
            if (k & 1) o = o * x % p;
            x = x * x % p;
        }
        return o;
    };
    a %= p, b %= p;
    T block = sqrtl(p);
    unordered_map<T, vector<T>> mp(block);
    for (T i = 0, pw = 1; i < block; ++i) {
        mp[pw].push_back(i);
        pw = pw * a % p;
    }
    const T inv_a = binpow(a, p - 2);
    const T iab = binpow(inv_a, block);
    vector<T> res;
    for (T k = 0, ipw = 1; k <= block; ++k, ipw = ipw * iab % p) {
        if (!all_solutions && res.size() == 1) break;
        T need = b * ipw % p;
        if (mp.count(need)) {
            for (T r : mp[need]) {
                if (k * block + r >= p) break;
                res.push_back(k * block + r);
                if (!all_solutions) break;
            }
        }
    }
    return res;
}
