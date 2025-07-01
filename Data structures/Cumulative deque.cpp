template<typename T, auto op>
class cumulative_deque {

    vector<T> vl, vr;
    vector<T> al, ar;

public:
    cumulative_deque(size_t n = 0) {
        vl.reserve(n);
        vr.reserve(n);
        al.reserve(n);
        ar.reserve(n);
    }

    bool empty() const {return vl.empty() && vr.empty();}
    size_t size() const {return vl.size() + vr.size();}
    void clear() {vl.clear(), vr.clear(); al.clear(), ar.clear();}

    void push_back(const T& x) {
        vr.push_back(x);
        ar.push_back(ar.empty() ? x : op(ar.back(), x));
    }

    void push_front(const T& x) {
        vl.push_back(x);
        al.push_back(al.empty() ? x : op(x, al.back()));
    }

    //O(1) op() calls amortized
    void pop_front() {
        assert(size());
        if (al.empty()) {
            const size_t szl = ar.size() / 2;
            vl.insert(vl.end(), vr.rbegin() + szl, vr.rend());
            vr.erase(vr.begin(), vr.end() - szl);
            al = vl, ar = vr;
            for (size_t i = 1; i < al.size(); ++i) al[i] = op(al[i], al[i - 1]);
            for (size_t i = 1; i < al.size(); ++i) ar[i] = op(ar[i - 1], ar[i]);
        }
        vl.pop_back();
        al.pop_back();
    }

    //O(1) op() calls amortized
    void pop_back() {
        assert(size());
        if (ar.empty()) {
            const size_t szr = al.size() / 2;
            vr.insert(vr.end(), vl.rbegin() + szr, vl.rend());
            vl.erase(vl.begin(), vl.end() - szr);
            al = vl, ar = vr;
            for (size_t i = 1; i < al.size(); ++i) al[i] = op(al[i], al[i - 1]);
            for (size_t i = 1; i < al.size(); ++i) ar[i] = op(ar[i - 1], ar[i]);
        }
        vr.pop_back();
        ar.pop_back();
    }

    T query() const {
        assert(size());
        if (vl.empty() || vr.empty()) return (vl.empty() ? ar : al).back();
        return op(al.back(), ar.back());
    }
};
