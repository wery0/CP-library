namespace GOD_FACTORIZATOR {
#pragma GCC push_options
#ifndef _CPLIB_NO_FORCE_BMI2_
#pragma GCC target("abm,bmi,bmi2")
#endif

    namespace cplib::port {

        namespace impl {

            template<typename T>
            constexpr bool is_unsigned_integer_v = std::is_unsigned_v<T> && !std::is_same_v<T, bool>;

            template<typename T>
            constexpr bool long_long_or_smaller = std::numeric_limits<T>::digits <= std::numeric_limits<unsigned long long>::digits;

            template<typename T>
            constexpr bool int_or_smaller = std::numeric_limits<T>::digits <= std::numeric_limits<unsigned int>::digits;

        }  // namespace impl

        template<typename T, std::enable_if_t<impl::is_unsigned_integer_v<T>>* = nullptr>
        constexpr int countl_zero(T x) noexcept {
            static_assert(impl::long_long_or_smaller<T>);
            if (x == 0) {
                return std::numeric_limits<T>::digits;
            } else if (impl::int_or_smaller<T>) {
                return __builtin_clz(x) - std::numeric_limits<unsigned int>::digits + std::numeric_limits<T>::digits;
            } else {
                return __builtin_clzll(x) - std::numeric_limits<unsigned long long>::digits + std::numeric_limits<T>::digits;
            }
        };

        template<typename T, std::enable_if_t<impl::is_unsigned_integer_v<T>>* = nullptr>
        constexpr int countr_zero(T x) noexcept {
            static_assert(impl::long_long_or_smaller<T>);
            if (x == 0) {
                return std::numeric_limits<T>::digits;
            } else if (impl::int_or_smaller<T>) {
                return __builtin_ctz(x);
            } else {
                return __builtin_ctzll(x);
            }
        };

        template<typename T, std::enable_if_t<impl::is_unsigned_integer_v<T>>* = nullptr>
        constexpr int bit_width(T x) noexcept {
            return std::numeric_limits<T>::digits - countl_zero(x);
        };

        template<typename T, std::enable_if_t<impl::is_unsigned_integer_v<T>>* = nullptr>
        constexpr T bit_floor(T x) noexcept {
            return x == 0 ? 0 : T(1) << (bit_width(x) - 1);
        };

        template<typename T, std::enable_if_t<impl::is_unsigned_integer_v<T>>* = nullptr>
        constexpr T bit_ceil(T x) noexcept {
            return x <= T(1) ? T(1) : T(1) << bit_width(x - 1);
        };

        template<typename T, std::enable_if_t<impl::is_unsigned_integer_v<T>>* = nullptr>
        constexpr bool has_single_bit(T x) noexcept {
            return x != 0 && (x & (x - 1)) == 0;
        };

        template<typename T, std::enable_if_t<impl::is_unsigned_integer_v<T>>* = nullptr>
        constexpr int popcount(T x) noexcept {
            static_assert(impl::long_long_or_smaller<T>);
            if (impl::int_or_smaller<T>) {
                return __builtin_popcount(x);
            } else {
                return __builtin_popcountll(x);
            }
        };

    }  // namespace cplib::port

#pragma GCC pop_options

    namespace cplib {

        template<typename T, std::enable_if_t<std::is_unsigned_v<T>>* = nullptr>
        constexpr T gcd(T x, T y) {
            if (x == 0 || y == 0) return x + y;
            int kx = port::countr_zero(x);
            int ky = port::countr_zero(y);
            x >>= kx;
            while (y != 0) {
                y >>= port::countr_zero(y);
                if (x > y) std::swap(x, y);
                y -= x;
            }
            return x << std::min(kx, ky);
        }

        template<typename T, std::enable_if_t<std::is_unsigned_v<T>>* = nullptr>
        constexpr std::tuple<std::make_signed_t<T>, std::make_signed_t<T>, T> bezout(T x, T y) {
            bool swap = x < y;
            if (swap) {
                std::swap(x, y);
            }
            if (y == 0) {
                if (x == 0) {
                    return {0, 0, 0};
                } else if (swap) {
                    return {0, 1, x};
                } else {
                    return {1, 0, x};
                }
            }
            using S = std::make_signed_t<T>;
            S s0 = 1, s1 = 0, t0 = 0, t1 = 1;
            while (true) {
                T q = x / y, r = x % y;
                if (r == 0) {
                    if (swap) {
                        return {t1, s1, y};
                    } else {
                        return {s1, t1, y};
                    }
                }
                S s2 = s0 - S(q) * s1, t2 = t0 - S(q) * t1;
                x = y;
                y = r;
                s0 = s1;
                s1 = s2;
                t0 = t1;
                t1 = t2;
            }
        }

        template<typename T, std::enable_if_t<std::is_unsigned_v<T>>* = nullptr>
        constexpr T mod_inverse(T x, T m) {
            auto [s, t, g] = bezout(x, m);
            assert(g == 1);
            return s < 0 ? T(s) + m : s;
        }

    }  // namespace cplib

    namespace cplib::impl {

        template<typename T> struct make_double_width {};
        template<> struct make_double_width<uint8_t> { using type = uint16_t; };
        template<> struct make_double_width<uint16_t> { using type = uint32_t; };
        template<> struct make_double_width<uint32_t> { using type = uint64_t; };
        template<> struct make_double_width<uint64_t> { using type = unsigned __int128; };

        template<typename T>
        using make_double_width_t = typename make_double_width<T>::type;

    }  // namespace cplib::impl

    namespace cplib {

        namespace impl {

            template<typename UInt>
            class MontgomeryReduction {
            public:
                using int_type = UInt;
                using int_double_t = make_double_width_t<int_type>;
                static constexpr int base_width = std::numeric_limits<int_type>::digits;

                constexpr explicit MontgomeryReduction(int_type mod) :
                    _mod(mod),
                    _mod_neg_inv(inv_base(~(mod - 1))),
                    _mbase((int_double_t(1) << base_width) % mod),
                    _mbase2(int_double_t(_mbase) * _mbase % mod),
                    _mbase3(int_double_t(_mbase2) * _mbase % mod)
                {}

                constexpr int_type mod() const { return _mod; }

                constexpr int_type mbase() const { return _mbase; }

                constexpr int_type mbase2() const { return _mbase2; }

                constexpr int_type mbase3() const { return _mbase3; }

                constexpr int_type reduce(int_double_t t) const {
                    int_type m = int_type(t) * _mod_neg_inv;
                    int_type r = (t + int_double_t(m) * _mod) >> base_width;
                    return r;
                }

                constexpr int_type shrink(int_type x) const {
                    return x >= _mod * 2 ? x - _mod * 2 : x;
                }

                constexpr int_type strict_shrink(int_type x) const {
                    return x >= _mod ? x - _mod : x;
                }

            private:
                int_type _mod, _mod_neg_inv, _mbase, _mbase2, _mbase3;

                static constexpr int_type inv_base(int_type x) {
                    int_type y = 1;
                    for (int i = 1; i < base_width; i *= 2) {
                        y *= int_type(2) - x * y;
                    }
                    return y;
                }
            };

        }  // namespace impl

        template<typename Context>
        class MontgomeryModInt {
        public:
            using mint = MontgomeryModInt;
            using int_type = typename Context::int_type;
            using mr_type = typename Context::mr_type;
            using int_double_t = typename mr_type::int_double_t;

            MontgomeryModInt() : _val(0) {}

            template < typename T, std::enable_if_t < std::is_integral_v<T> && std::is_signed_v<T >>* = nullptr >
            explicit MontgomeryModInt(T x) {
                using signed_int_type = std::make_signed_t<int_type>;
                signed_int_type v = x % signed_int_type(mr().mod());
                _val = mr().reduce(mr().mbase2() * int_double_t(v < 0 ? v + mr().mod() : v));
            }

            template<typename T, std::enable_if_t<std::is_unsigned_v<T>>* = nullptr>
            explicit MontgomeryModInt(T x) {
                _val = mr().reduce(mr().mbase2() * int_double_t(x % mr().mod()));
            }

            int_type val() const {
                return mr().strict_shrink(mr().reduce(_val));
            }

            int_type residue() const {
                return mr().strict_shrink(_val);
            }

            static constexpr int_type mod() { return mr().mod(); }

            mint& operator++() {
                _val = mr().shrink(_val + mr().mbase());
                return *this;
            }

            mint operator++(int) {
                mint ret = *this;
                ++(*this);
                return ret;
            }

            mint operator+() const {
                return *this;
            }

            mint& operator+=(const mint &rhs) {
                _val = mr().shrink(_val + rhs._val);
                return *this;
            }

            mint operator+(const mint &rhs) const {
                mint ret = *this;
                ret += rhs;
                return ret;
            }

            mint& operator--() {
                _val = mr().shrink(_val + mr().mod() - mr().mbase());
                return *this;
            }

            mint operator--(int) {
                mint ret = *this;
                --(*this);
                return ret;
            }

            mint operator-() const {
                return from_raw(_val == 0 ? 0 : mr().mod() * 2 - _val);
            }

            mint& operator-=(const mint &rhs) {
                _val = mr().shrink(_val + mr().mod() * 2 - rhs._val);
                return *this;
            }

            mint operator-(const mint &rhs) const {
                mint ret = *this;
                ret -= rhs;
                return ret;
            }

            mint& operator*=(const mint &rhs) {
                _val = mr().reduce(int_double_t(_val) * rhs._val);
                return *this;
            }

            mint operator*(const mint &rhs) const {
                mint ret = *this;
                ret *= rhs;
                return ret;
            }

            mint inv() const {
                return from_raw(mr().reduce(int_double_t(mr().mbase3()) * mod_inverse(_val, mr().mod())));
            }

            mint& operator/=(const mint &rhs) {
                return *this *= rhs.inv();
            }

            mint operator/(const mint &rhs) const {
                mint ret = *this;
                ret /= rhs;
                return ret;
            }

            bool operator==(const mint &rhs) const {
                return mr().strict_shrink(_val) == mr().strict_shrink(rhs._val);
            }

            bool operator!=(const mint &rhs) const {
                return !(*this == rhs);
            }

        private:
            int_type _val;

            static constexpr mint from_raw(int_type x) {
                mint ret;
                ret._val = x;
                return ret;
            }

            static constexpr const mr_type& mr() {
                return Context::montgomery_reduction();
            }
        };

        template<typename UInt, UInt Mod, std::enable_if_t<std::is_unsigned_v<UInt>>* = nullptr>
        class StaticMontgomeryReductionContext {
        public:
            using int_type = UInt;
            using mr_type = impl::MontgomeryReduction<int_type>;
            static_assert(Mod % 2 == 1 && Mod <= std::numeric_limits<int_type>::max() / 4);

            static constexpr const mr_type& montgomery_reduction() {
                return _reduction;
            }

        private:
            static constexpr auto _reduction = mr_type(Mod);
        };

        template<typename UInt, std::enable_if_t<std::is_unsigned_v<UInt>>* = nullptr>
        class DynamicMontgomeryReductionContext {
        public:
            using int_type = UInt;
            using mr_type = impl::MontgomeryReduction<int_type>;

            struct Guard {
                Guard(const Guard&) = delete;
                Guard(Guard&&) = delete;
                Guard& operator=(const Guard&) = delete;
                Guard& operator=(Guard&&) = delete;
                ~Guard() {
                    _reduction_env.pop_back();
                }
            private:
                friend DynamicMontgomeryReductionContext;
                Guard() {};
            };

            [[nodiscard]]
            static Guard set_mod(int_type mod) {
                assert(mod % 2 == 1 && mod <= std::numeric_limits<int_type>::max() / 4);
                _reduction_env.emplace_back(mod);
                return Guard();
            }

            static constexpr const mr_type& montgomery_reduction() {
                return _reduction_env.back();
            }

        private:
            static inline std::vector<mr_type> _reduction_env;
        };

        template<uint32_t Mod>
        using MMInt = MontgomeryModInt<StaticMontgomeryReductionContext<uint32_t, Mod>>;

        template<uint64_t Mod>
        using MMInt64 = MontgomeryModInt<StaticMontgomeryReductionContext<uint64_t, Mod>>;

    }  // namespace cplib

    namespace cplib {

        template<typename T>
        constexpr T pow(T base, unsigned long long exp) {
            T res(1);
            while (exp) {
                if (exp & 1) res *= base;
                base *= base;
                exp >>= 1;
            }
            return res;
        }

    }  // namespace cplib

    namespace cplib {

        namespace impl {

            template<typename ModInt, typename T = typename ModInt::int_type>
            T miller_rabin(ModInt a, T d, int r) {
                const ModInt one(1), minus_one(-1);
                ModInt x = pow(a, d);
                if (x == one || x == minus_one) return 1;
                while (r--) {
                    ModInt y = x * x;
                    if (y == one) return gcd(x.val() - 1, ModInt::mod());
                    x = y;
                    if (x == minus_one) return 1;
                }
                return 0;
            }

            constexpr uint64_t small_primes_mask() {
                uint64_t mask = 0;
                for (int i : {2, 3 , 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61}) {
                    mask |= 1ull << i;
                }
                return mask;
            }

            constexpr bool is_prime_lt64(int n) {
                return (1ull << n) & small_primes_mask();
            }

            static uint32_t prime_or_factor_32(uint32_t n) {
                if (n < 64) return is_prime_lt64(n);
                if (n % 2 == 0) return 2;
                constexpr uint32_t small_prod = 3u * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29;
                uint32_t g = gcd(n, small_prod);
                if (g != 1) return g != n ? g : 0;
                using ctx = DynamicMontgomeryReductionContext<uint32_t>;
                auto _guard = ctx::set_mod(n);
                using mint = MontgomeryModInt<ctx>;
                int r = port::countr_zero(n - 1);
                uint32_t d = (n - 1) >> r;
                for (uint32_t a : {2, 7, 61}) {
                    uint32_t ret = miller_rabin(mint(a), d, r);
                    if (ret != 1) return ret;
                }
                return 1;
            }

            static uint64_t prime_or_factor_64(uint64_t n) {
                if (n < 64) return is_prime_lt64(n);
                if (n % 2 == 0) return 2;
                constexpr uint64_t small_prod = 3ull * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47 * 53;
                uint64_t g = gcd(n, small_prod);
                if (g != 1) return g != n ? g : 0;
                using ctx = DynamicMontgomeryReductionContext<uint64_t>;
                auto _guard = ctx::set_mod(n);
                using mint = MontgomeryModInt<ctx>;
                int r = port::countr_zero(n - 1);
                uint64_t d = (n - 1) >> r;
                for (uint64_t a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
                    uint64_t ret = miller_rabin(mint(a), d, r);
                    if (ret != 1) return ret;
                }
                return 1;
            }

        }  // namespace impl

        template<typename T, std::enable_if_t<std::is_unsigned_v<T>>* = nullptr>
        T prime_or_factor(T n) {
            if (n < (1 << 30)) {
                return impl::prime_or_factor_32(n);
            } else {
                return impl::prime_or_factor_64(n);
            }
        }

        template<typename T, std::enable_if_t<std::is_unsigned_v<T>>* = nullptr>
        bool is_prime(T n) {
            return prime_or_factor(n) == 1;
        }

    }  // namespace cplib

    namespace cplib {

        namespace impl {

            template<typename T>
            struct FactorizationResult {
                std::vector<T> factors, prime_factors;
            };

            int jacobi(long long n, long long k) {
                assert(k > 0 && k % 2 == 1);
                n %= k;
                int t = 1;
                while (n != 0) {
                    while (n % 2 == 0) {
                        n /= 2;
                        int r = k % 8;
                        if (r == 3 || r == 5) t = -t;
                    }
                    std::swap(n, k);
                    if (n % 4 == 3 && k % 4 == 3) t = -t;
                    n %= k;
                }
                return k == 1 ? t : 0;
            }

            template<typename ModInt, typename T = typename ModInt::int_type>
            struct MontgomeryCurve {
                struct Point {
                    ModInt x, z;
                    T check() const {
                        return gcd(z.val(), ModInt::mod());
                    }
                };
                MontgomeryCurve(ModInt a) : _a24((a + ModInt(2)) / ModInt(4)) {}
                static std::pair<MontgomeryCurve, Point> random_curve_and_point() {
                    while (true) {
                        ModInt a(std::rand()),
                               x(std::rand()),
                               m1(1),
                               y2 = x * (x * (x + a) + m1);
                        if (jacobi(y2.val(), ModInt::mod()) == 1) {
                            return {MontgomeryCurve(a), Point{x, m1}};
                        }
                    }
                }
                Point dbl(const Point &p) const {
                    ModInt s = p.x + p.z,
                           d = p.x - p.z,
                           s2 = s * s,
                           d2 = d * d,
                           t = s2 - d2,
                           x = s2 * d2,
                           z = t * (d2 + _a24 * t);
                    return {x, z};
                }
                Point add(const Point &p, const Point &q, const Point &diff) const {
                    ModInt u = (p.x - p.z) * (q.x + q.z),
                           v = (p.x + p.z) * (q.x - q.z),
                           u_plus_v = u + v,
                           u_minus_v = u - v,
                           x = diff.z * u_plus_v * u_plus_v,
                           z = diff.x * u_minus_v * u_minus_v;
                    return {x, z};
                }
                Point mul(const Point &p, unsigned long long k) const {
                    Point p0 = p, p1 = dbl(p);
                    for (int b = port::bit_width(k) - 2; b >= 0; --b) {
                        Point tmp = add(p1, p0, p);
                        if ((k >> b) & 1) {
                            p1 = dbl(p1);
                            p0 = tmp;
                        } else {
                            p0 = dbl(p0);
                            p1 = tmp;
                        }
                    }
                    return p0;
                }
            private:
                ModInt _a24;
            };

            std::vector<unsigned long long> ecm_blocks(int smooth_bound) {
                std::vector<bool> sieve(smooth_bound + 1, true);
                std::vector<unsigned long long> blocks{1};
                for (int p = 2; p <= smooth_bound; ++p) {
                    if (sieve[p]) {
                        int pw = p;
                        while (pw <= smooth_bound) {
                            if (blocks.back() <= std::numeric_limits<unsigned long long>::max() / p) {
                                blocks.back() *= p;
                            } else {
                                blocks.push_back(p);
                            }
                            pw *= p;
                        }
                        for (int i = p * p; i <= smooth_bound; i += p) {
                            sieve[i] = false;
                        }
                    }
                }
                return blocks;
            }

            template<typename ModInt, typename T = typename ModInt::int_type>
            T ecm_modint() {
                const int B1 = 400, B2 = 3000;
                static const std::vector<unsigned long long> blocks = ecm_blocks(B1);
                while (true) {
                    auto [curve, point] = MontgomeryCurve<ModInt>::random_curve_and_point();
                    T f = 1;
                    // Stage 1
                    for (unsigned long long blk : blocks) {
                        auto new_point = curve.mul(point, blk);
                        f = new_point.check();
                        if (f != 1) {
                            if (f != ModInt::mod()) {
                                return f;
                            } else {
                                break;
                            }
                        }
                        point = new_point;
                    }
                    if (f == ModInt::mod()) {
                        continue;
                    }
                    // Stage 2
                    auto six = curve.dbl(curve.add(curve.dbl(point), point, point));
                    auto q0 = six, q1 = curve.dbl(six);
                    for (int i = 6; i < B1; i += 6) {
                        q0 = curve.add(q1, six, q0);
                        std::swap(q0, q1);
                    }
                    ModInt xprod(1);
                    ModInt x_norm = point.x / point.z;
                    for (int i = B1 / 6 * 6; i < B2; i += 6) {
                        xprod *= q0.x - q0.z * x_norm;
                        if (i % 300 == 0) {
                            f = gcd(xprod.val(), ModInt::mod());
                            if (f != 1) {
                                if (f != ModInt::mod()) {
                                    return f;
                                } else {
                                    break;
                                }
                            }
                        }
                        q0 = curve.add(q1, six, q0);
                        std::swap(q0, q1);
                    }
                    if (f == 1) {
                        f = gcd(xprod.val(), ModInt::mod());
                        if (f != 1 && f != ModInt::mod()) {
                            return f;
                        }
                    }
                }
            }

            template<typename T>
            T ecm(T n) {
                for (int k = 2; k < port::bit_width(n); ++k) {
                    T r = roundl(powl(n, 1.0l / k)), pw = r;
                    for (int i = 1; i < k; ++i) pw *= r;
                    if (pw == n) return r;
                }
                using ctx = DynamicMontgomeryReductionContext<T>;
                auto _guard = ctx::set_mod(n);
                using mint = MontgomeryModInt<ctx>;
                return ecm_modint<mint>();
            }

            // https://maths-people.anu.edu.au/~brent/pd/rpb051i.pdf
            template<typename ModInt, typename T = typename ModInt::int_type>
            T pollard_rho_modint() {
                const T n = ModInt::mod();
                constexpr T m = std::numeric_limits<T>::digits;
                T r = 1, g;
                ModInt c(0), y, q, x, ys;
                do {
                    ++c;
                    y = ModInt(2);
                    q = ModInt(1);
                    g = 1;
                    do {
                        x = y;
                        for (T i = 0; i < r; ++i) y = y * y + c;
                        ys = y;
                        for (T i = 0; i < r; ++i) {
                            y = y * y + c;
                            q *= y - x;
                            if ((i + 1) % m == 0) {
                                g = gcd(q.val(), n);
                                if (g != 1) break;
                                ys = y;
                            }
                        }
                        if (g == 1 && r % m != 0) {
                            g = gcd(q.val(), n);
                        }
                        r *= 2;
                    } while (g == 1);
                    if (g == n) {
                        do {
                            ys = ys * ys + c;
                            g = gcd((ys - x).val(), n);
                        } while (g == 1);
                    }
                } while (g == n);
                return g;
            }

            template<typename T>
            T pollard_rho(T n) {
                using ctx = DynamicMontgomeryReductionContext<T>;
                auto _guard = ctx::set_mod(n);
                using mint = MontgomeryModInt<ctx>;
                return pollard_rho_modint<mint>();
            }

            template<typename T>
            void factorize_work(FactorizationResult<T> &result) {
                T n = result.factors.back();
                result.factors.pop_back();
                T f = prime_or_factor(n);
                if (f == 1) {
                    result.prime_factors.push_back(n);
                    return;
                }
                if (f == 0) {
                    if (n < (1 << 30)) {
                        f = pollard_rho<uint32_t>(n);
                    } else if (n < (1ull << 59)) {
                        f = pollard_rho<uint64_t>(n);
                    } else {
                        f = ecm<uint64_t>(n);
                    }
                }
                result.factors.push_back(f);
                result.factors.push_back(n / f);
            }

        }  // namespace impl

        template<typename T, std::enable_if_t<std::is_unsigned_v<T>>* = nullptr>
        std::vector<T> factorize(T n) {
            if (n <= 1) return {};
            int twos = port::countr_zero(n);
            impl::FactorizationResult<T> result;
            result.prime_factors.insert(result.prime_factors.end(), twos, 2);
            if (port::has_single_bit(n)) {
                return result.prime_factors;
            }
            result.factors.push_back(n >> twos);
            while (!result.factors.empty()) {
                impl::factorize_work(result);
            }
            std::sort(result.prime_factors.begin(), result.prime_factors.end());
            return result.prime_factors;
        }

    }  // namespace cplib

    //n = p_1 ^ k_1 * p_2 ^ k_2 *... Returns vector of pairs {p_i, k_i}
    //Factorizes numbers up to 2^62-1 ultra fast
    //Complexity: O(n ^ {1 / 4}) expected
    template<typename T, std::enable_if_t<std::is_unsigned_v<T>>* = nullptr>
    std::vector<array<T, 2>> factorize(T n) {
        vector<array<T, 2>> res;
        T pr = 0, c = 0;
        for (auto p : cplib::factorize(n)) {
            if (pr == p) ++c;
            else {
                if (c) res.push_back({pr, c});
                c = 1;
            }
            pr = p;
        }
        if (c) res.push_back({pr, c});
        return res;
    }
};
