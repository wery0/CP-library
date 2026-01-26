//Alias (aka Walker's) method for static weighted random sampling
//Preprocessing: <O(n), O(n)>
//Querying for random element: O(1)
//Constraints: max(weights) * n should fit into W
template<typename W, typename D = long double>
class alias_method {
    static_assert(is_floating_point_v<D>);

    size_t n;
    vector<D> p;
    vector<int> alias;
    uniform_int_distribution<int> g1;
    uniform_real_distribution<D> g2;

public:
    alias_method(vector<W> weights): n(weights.size()), p(n, 1), alias(n, -1), g1(0, n - 1) {
        assert(n);
        W avg = 0;
        for (W& w : weights) {
            assert(w >= 0);
            avg += w;
            w *= n;
        }
        vector<int> idx(n); iota(idx.begin(), idx.end(), 0);
        int cs = partition(idx.begin(), idx.end(), [&](int i) {return weights[i] < avg;}) - idx.begin();
        for (; cs && idx.size() > cs;) {
            int b = idx.back(), s = idx[cs - 1];
            swap(idx.back(), idx[cs - 1]); idx.pop_back();
            alias[s] = b; p[s] = D(weights[s]) / avg;
            weights[b] -= avg - weights[s];
            if (weights[b] >= avg) --cs;
        }
    }

    //Returns i with probability weights[i] / sum(weights)
    int sample() {
        static mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
        // D x = g2(rnd); int i = n * x;
        // if (alias[i] == -1) return i;
        // return n * x - i < p[i] ? i : alias[i];
        int i = g1(rnd);
        if (alias[i] == -1) return i;
        return g2(rnd) < p[i] ? i : alias[i];
    }
};
