namespace SternBrocotTree {
    template<typename T = int64_t>
    class Sfraction {
        static_assert(is_integral_v<T> && is_signed_v<T>, "Numeric type must be signed and integral");

        T numerator_ = 0;
        T denominator_ = 1;

        static T binpow(T val, T pow) {
            T res = 1;
            for (T x = val; pow; pow >>= 1) {
                if (pow & 1) res *= x;
                x *= x;
            }
            return res;
        }

        void normalize() {
            assert((denominator_ || numerator_) && "0 / 0 is not allowed!");
            if (denominator_ < 0) {
                denominator_ *= -1;
                numerator_ *= -1;
            }
        }

    public:
        Sfraction() = default;

        template<typename U>
        Sfraction(U value): numerator_(value) {
            static_assert(is_integral_v<U> && is_signed_v<U>, "Numeric type must be signed and integral");
        }

        template <typename U>
        Sfraction(U numerator, U denominator): numerator_(numerator), denominator_(denominator) {
            static_assert(is_integral_v<U> && is_signed_v<U>, "Numeric type must be signed and integral");
            assert((denominator_ || numerator_) && "0 / 0 is not allowed");
            normalize();
        }

        Sfraction& operator+=(const Sfraction& rhs) {
            numerator_ = numerator_ * rhs.denominator_ + rhs.numerator_ * denominator_;
            denominator_ *= rhs.denominator_;
            normalize();
            return *this;
        }
        friend Sfraction operator+(const Sfraction& lhs, const Sfraction& rhs) {
            Sfraction res = lhs;
            res += rhs;
            return res;
        }

        Sfraction& operator-=(const Sfraction& rhs) {
            numerator_ = numerator_ * rhs.denominator_ - rhs.numerator_ * denominator_;
            denominator_ *= rhs.denominator_;
            normalize();
            return *this;
        }
        friend Sfraction operator-(const Sfraction& lhs, const Sfraction& rhs) {
            Sfraction res = lhs;
            res -= rhs;
            return res;
        }

        Sfraction& operator*=(const Sfraction& rhs) {
            numerator_ *= rhs.numerator_;
            denominator_ *= rhs.denominator_;
            normalize();
            return *this;
        }
        friend Sfraction operator*(const Sfraction& lhs, const Sfraction& rhs) {
            Sfraction res = lhs;
            res *= rhs;
            return res;
        }

        Sfraction& operator/=(const Sfraction& rhs) {
            assert((!is_zero() || !rhs.is_zero()) && "0 / 0 is not allowed!");
            numerator_ *= rhs.denominator_;
            denominator_ *= rhs.numerator_;
            normalize();
            return *this;
        }
        friend Sfraction operator/(const Sfraction& lhs, const Sfraction& rhs) {
            Sfraction res = lhs;
            res /= rhs;
            return res;
        }

        Sfraction operator-() const { return { -numerator_, denominator_}; }

        bool operator==(const Sfraction& rhs) const { return numerator_ == rhs.numerator_ && denominator_ == rhs.denominator_; }
        friend bool operator!=(const Sfraction& lhs, const Sfraction& rhs) { return !(lhs == rhs); }
        bool operator<(const Sfraction& rhs) const { return numerator_ * rhs.denominator_ < rhs.numerator_ * denominator_; }
        friend bool operator>(const Sfraction& lhs, const Sfraction& rhs) { return rhs < lhs; }
        friend bool operator<=(const Sfraction& lhs, const Sfraction& rhs) { return !(rhs < lhs); }
        friend bool operator>=(const Sfraction& lhs, const Sfraction& rhs) { return !(lhs < rhs); }

        void pow(T power) {
            if (numerator_ == 0) return;
            T sign = numerator_ > 0 ? 1 : -1;
            numerator_ *= sign;
            if (power < 0) {
                swap(numerator_, denominator_);
                power *= -1;
            }
            numerator_ = binpow(numerator_, power);
            denominator_ = binpow(denominator_, power);
            if ((power & 1) && sign == -1) numerator_ *= -1;
        }
        friend Sfraction pow(const Sfraction& rhs, T power) {
            Sfraction res = rhs;
            res.pow(power);
            return res;
        }

        void invert() {swap(numerator_, denominator_);}
        friend Sfraction invert(const Sfraction& rhs) {
            Sfraction res = rhs;
            res.invert();
            return res;
        }

        friend Sfraction mediant(const Sfraction& lhs, const Sfraction& rhs) {
            return {lhs.numerator_ + rhs.numerator_, lhs.denominator_ + rhs.denominator_};
        }

        bool is_zero() const {return numerator_ == 0;}
        bool is_infinity() const {return denominator_ == 0;}
        bool is_integer() const {return denominator_ == 1;}
        int get_sign() const {return numerator_ < 0 ? -1 : numerator_ == 0 ? 0 : 1;}
        T get_numerator() const {return numerator_;}
        T get_denominator() const {return denominator_;}
        friend Sfraction abs(const Sfraction& rhs) {return rhs.numerator_ > 0 ? rhs : -rhs;}

        friend ostream& operator<<(ostream& os, const Sfraction& rhs) {
            if (rhs.is_infinity()) return os << "1 0";
            if (rhs == -1) return os << "-1";
            return os <<  rhs.numerator_ << " " << rhs.denominator_;
        }
    };

    //Returns {path from root of Stern-Brocot tree to the f, range of vertex f in Stern-Brocot tree}
    //Complexity: O(log(min(f.numerator, f.denominator)))
    //Example: encode_path(5 / 9) = {{1, 1, 3}, {1 / 2, 4 / 7}} which means that you need to go left * 1, right * 1, left * 3 to reach 5 / 9
    template<typename T>
    pair<vector<T>, array<Sfraction<T>, 2>> encode_path(Sfraction<T> f) {
        assert(f >= 0 && "No path!");
        assert(f > 0 && !f.is_infinity() && "Infinite path!");
        bool was_inverted = false;
        if (f > 1) was_inverted = true, f.invert();
        vector<T> ans;
        Sfraction<T> l = {0, 1}, r = {1, 0};
        auto go_left = [&](T step) {r = {l.get_numerator() * step + r.get_numerator(), l.get_denominator() * step + r.get_denominator()};};
        auto go_right = [&](T step) {l = {l.get_numerator() + r.get_numerator() * step, l.get_denominator() + r.get_denominator() * step};};
        while (true) {
            if (mediant(l, r) == f) return {ans, {l, r}};
            T ch = f.get_numerator() * r.get_denominator() - f.get_denominator() * r.get_numerator();
            T zn = f.get_denominator() * l.get_numerator() - f.get_numerator() * l.get_denominator();
            T x = zn < 0 ? (-ch - 1) / -zn : (ch - 1) / zn;
            ans.push_back(x);
            go_left(x);

            if (mediant(l, r) == f) return {ans, {l, r}};
            ch = f.get_numerator() * l.get_denominator() - f.get_denominator() * l.get_numerator();
            zn = f.get_denominator() * r.get_numerator() - f.get_numerator() * r.get_denominator();
            x = zn < 0 ? (-ch - 1) / -zn : (ch - 1) / zn;
            ans.push_back(x);
            go_right(x);
        }
        assert(0);
        return {};
    }

    //Complexity: O(|steps|)
    template<typename T>
    Sfraction<T> decode_path(vector<T> steps, bool is_left_first) {
        Sfraction<T> l = {0, 1}, r = {1, 0};
        if (steps.empty()) return mediant(l, r);
        auto go_left = [&](T step) {r = {l.get_numerator() * step + r.get_numerator(), l.get_denominator() * step + r.get_denominator()};};
        auto go_right = [&](T step) {l = {l.get_numerator() + r.get_numerator() * step, l.get_denominator() + r.get_denominator() * step};};
        if (!is_left_first) go_right(steps[0]);
        if (!is_left_first ^ (steps.size() & 1)) steps.push_back(0);
        for (size_t i = !is_left_first; i < steps.size(); i += 2) {
            go_left(steps[i]);
            go_right(steps[i + 1]);
        }
        return mediant(l, r);
    }

    //Complexity: O(log(max(min(x.numerator, x.denominator), min(y.numerator, y.denominator))))
    template<typename T>
    Sfraction<T> lca(Sfraction<T> x, Sfraction<T> y) {
        if (x > y) swap(x, y);
        if (x <= 1 && y >= 1) return 1;
        auto px = encode_path(x).first;
        auto py = encode_path(y).first;
        if (px > py) swap(px, py);
        auto [ix, iy] = mismatch(px.begin(), px.end(), py.begin(), py.end());
        if (ix != px.end()) px.resize(ix - px.begin() + 1);
        return decode_path(px, x < 1);
    }

    //Returns kth ancestor of f in Stern-Brocot tree
    //Complexity: O(log(min(f.numerator, f.denominator)))
    template<typename T>
    Sfraction<T> kth_ancestor(Sfraction<T> f, int64_t k) {
        if (k == 0) return 1;
        auto p = encode_path(f).first;
        for (size_t i = 0; i < p.size(); ++i) {
            if (k <= p[i]) {
                p[i] = k;
                p.resize(i + 1);
                return decode_path(p, f < 1);
            }
            k -= p[i];
        }
        return -1;
    }

    //Returns range of values which vertex with value f represents in Stern-Brocot tree
    //Complexity: O(log(min(f.numerator, f.denominator)))
    template<typename T>
    array<Sfraction<T>, 2> range(Sfraction<T> f) {
        auto [p, gr] = encode_path(f);
        auto [l, r] = gr;
        if (f > 1) {
            l.invert();
            r.invert();
            swap(l, r);
        }
        return {l, r};
    }
}
using namespace SternBrocotTree;
