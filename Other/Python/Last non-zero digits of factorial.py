def binpow(x, pw, mod):
    o = 1
    for i in str(bin(pw)[2:])[::-1]:
        if i == '1':
            o = o * x % mod
        x = x * x % mod
    return o


# O(5^{d/3} * log_5{n}) arithmetic operations
def calc_last_d_nonzero_digits_of_factorial(n, d):
    pw = (d + 2) // 3
    MOD = 10 ** d
    k = 5 ** pw
    kek = 1
    for q in range(0, k, 5):
        kek *= (q + 1) * (q + 2) * (q + 3) * (q + 4) // 2
        kek %= MOD
    arr = []
    while n:
        itr = n % k
        n -= itr
        res = 1
        for i in range(itr):
            res *= n + i + 1
            while res and res % 10 == 0: res //= 10
            res %= MOD
        res = res * binpow(kek, n // k, MOD) % MOD
        arr.append(res)
        n //= 5
    res = 1
    for i in arr:
        res = res * i
        while res and res % 10 == 0: res //= 10
        res %= MOD
    return res
