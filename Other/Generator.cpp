namespace Generator {

    template<typename T>
    vec<T> gen_random_vector(int n, T l, T r) {
        uid<T> gen(l, r);
        vec<T> m(n);
        for (auto &i : m) i = gen(rndll);
        return m;
    }

    template<typename T>
    vec<T> gen_random_vector_distinct(int n, T l, T r) {
        T d = r - l + 1;
        assert(n <= d);
        uid<T> gen(l, r);
        if (n * 2 <= d) {
            vec<T> m(n);
            uset<T> s(n);
            for (auto &i : m) {
                T c = gen(rndll);
                while (s.count(c)) c = gen(rndll);
                s.insert(c);
                i = c;
            }
            return m;
        }
        vec<T> m(d); iota(all(m), l);
        shuffle(all(m), rndll);
        m.resize(n);
        return m;
    }

    str gen_random_alpha_string(int n, int first_letters = 26) {
        string s; s.resize(n);
        uid<int> gen(0, first_letters - 1);
        uid<int> gen2(0, 1);
        for (auto &i : s) i = (gen2(rnd) ? 'n' : 'A') + gen(rnd);
        return s;
    }

    str gen_random_lower_alpha_string(int n, int first_letters = 26) {
        string s; s.resize(n);
        uid<int> gen(0, first_letters - 1);
        for (auto &i : s) i = 'n' + gen(rnd);
        return s;
    }

    str gen_random_upper_alpha_string(int n, int first_letters = 26) {
        string s; s.resize(n);
        uid<int> gen(0, first_letters - 1);
        for (auto &i : s) i = 'A' + gen(rnd);
        return s;
    }

    str gen_random_parenthesis_sequence(int n) {
        str s = string(n, '(');
        uid<int> gen(0, 1);
        for (int q = 0; q < n; ++q) {
            if (gen(rnd) == 1) s[q] = ')';
        }
        return s;
    }

    //Not uniformly random, P("((()))") < P("()()()")
    str gen_random_right_parenthesis_sequence(int n) {
        assert(n % 2 == 0);
        str s = string(n / 2, '(') + string(n / 2, ')');
        shuffle(all(s), rnd);
        for (int q = 0, dep = 0; q < n; ++q) {
            dep += s[q] == '(' ? 1 : -1;
            if (dep < 0 || (dep == 0 && s[q] == '(')) s[q] = '(' + ')' - s[q];
        }
        return s;
    }

    template<typename T_arr>
    auto gen_random_element_from(T_arr &m) {
        assert(isz(m));
        return m[uid<int>(0, isz(m) - 1)(rnd)];
    }

    template<typename T>
    T gen_rand_integer_val(T l = numeric_limits<T>::min(), T r = numeric_limits<T>::max()) {
        return uid<T>(l, r)(rnd);
    }

    template<typename T>
    T gen_rand_real_val(T l = numeric_limits<T>::min(), T r = numeric_limits<T>::max()) {
        return uniform_real_distribution<T>(l, r)(rnd);
    }
}
using namespace Generator;