//All functions are uniformly random, unless other is stated
namespace Generator {

#define nlmin numeric_limits<T>::min() / 2
#define nlmax numeric_limits<T>::max() / 2

    template<typename T>
    T gen_val(T l = nlmin, T r = nlmax) {
        if constexpr(is_integral_v<T>) {
            static uniform_int_distribution<T> gint(l, r);
            return gint(rndll);
        } else if constexpr(is_floating_point_v<T>) {
            static uniform_real_distribution<T> greal(l, r);
            return greal(rndll);
        } else {
            assert(0);
        }
        return 0;
    }

    template<typename T>
    vector<T> gen_vector(int n, T l = nlmin, T r = nlmax) {
        vector<T> m(n);
        for (auto &i : m) i = gen_val<T>(l, r);
        return m;
    }

    template<typename T>
    vector<T> gen_vector_distinct(int n, T l = nlmin, T r = nlmax) {
        T d = r - l + 1;
        assert(n <= d);
        if (n * 2 <= d) {
            vector<T> m(n);
            uset<T> s(n);
            for (auto &i : m) {
                T c = gen_val<T>(l, r);
                while (s.count(c)) c = gen_val<T>(l, r);
                s.insert(c);
                i = c;
            }
            return m;
        }
        vector<T> m(d); iota(all(m), l);
        shuffle(all(m), rndll);
        m.resize(n);
        return m;
    }

    template<typename T>
    vector<vector<T>> gen_matrix(int n, int m, T l = nlmin, T r = nlmax) {
        vector<vector<T>> res(n, vector<T>(m));
        for (int q = 0; q < n; ++q) {
            for (int w = 0; w < m; ++w) {
                res[q][w] = gen_val<T>(l, r);
            }
        }
        return res;
    }

    string gen_alpha_string(int n, int first_letters = 26) {
        string s; s.resize(n);
        for (auto &i : s) i = (gen_val<int>(0, 1) ? 'a' : 'A') + gen_val<int>(0, first_letters - 1);
        return s;
    }

    string gen_lowercase_string(int n, int first_letters = 26) {
        string s; s.resize(n);
        for (auto &i : s) i = 'a' + gen_val<int>(0, first_letters - 1);
        return s;
    }

    string gen_uppercase_string(int n, int first_letters = 26) {
        string s; s.resize(n);
        for (auto &i : s) i = 'A' + gen_val<int>(0, first_letters - 1);
        return s;
    }

    string gen_palindrome_lowercase_string(int n, int first_letters = 26) {
        string s = gen_lowercase_string(n / 2, first_letters);
        if (n & 1) s.push_back('a' + gen_val(0, first_letters - 1));
        string t = s;
        reverse(all(t));
        return s + t;
    }

    string gen_parenthesis_sequence(int n) {
        string s = string(n, '(');
        for (int q = 0; q < n; ++q) {
            if (gen_val<int>(0, 1) == 1) s[q] = ')';
        }
        return s;
    }

    //Not uniformly random, P("((()))") < P("()()()")
    string gen_right_parenthesis_sequence(int n) {
        assert(n % 2 == 0);
        string s = string(n / 2, '(') + string(n / 2, ')');
        shuffle(all(s), rnd);
        for (int q = 0, dep = 0; q < n; ++q) {
            dep += s[q] == '(' ? 1 : -1;
            if (dep < 0 || (dep == 0 && s[q] == '(')) s[q] = '(' + ')' - s[q];
        }
        return s;
    }

    template<typename T_arr>
    auto gen_element_from(T_arr &m) {
        assert(!m.empty());
        return m[gen_val<size_t>(0, m.size() - 1)];
    }
}
using namespace Generator;