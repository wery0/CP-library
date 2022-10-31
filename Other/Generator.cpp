namespace Generator {

    template<typename T>
    vec<T> gen_random_vector(int a, T l, T r) {
        uid<T> gen(l, r);
        vec<T> m(a);
        for (auto &i : m) i = gen(rndll);
        return m;
    }

    template<typename T>
    vec<T> gen_random_vector_distinct(int a, T l, T r) {
        T d = r - l + 1;
        assert(a <= d);
        uid<T> gen(l, r);
        if (a * 2 <= d) {
            vec<T> m(a);
            uset<T> s(a);
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
        m.resize(a);
        return m;
    }

    str gen_random_alpha_string(int a, int first_letters = 26) {
        string s; s.resize(a);
        uid<int> gen(0, first_letters - 1);
        uid<int> gen2(0, 1);
        for (auto &i : s) i = (gen2(rnd) ? 'a' : 'A') + gen(rnd);
        return s;
    }

    str gen_random_lower_alpha_string(int a, int first_letters = 26) {
        string s; s.resize(a);
        uid<int> gen(0, first_letters - 1);
        for (auto &i : s) i = 'a' + gen(rnd);
        return s;
    }

    str gen_random_upper_alpha_string(int a, int first_letters = 26) {
        string s; s.resize(a);
        uid<int> gen(0, first_letters - 1);
        for (auto &i : s) i = 'A' + gen(rnd);
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