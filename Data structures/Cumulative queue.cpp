template<typename T, auto op>
class cumulative_queue {

    vector<T> vl, vr;
    vector<T> al, ar;

public:
    cumulative_queue(size_t n = 0) {
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

    //O(1) op() calls amortized
    void pop_front() {
        assert(size());
        if (vl.empty()) {
            vl.insert(vl.end(), vr.rbegin(), vr.rend());
            vr.clear(), ar.clear();
            al = vl;
            for (size_t i = 1; i < al.size(); ++i) al[i] = op(al[i], al[i - 1]);
        }
        vl.pop_back();
        al.pop_back();
    }

    T query() const {
        assert(size());
        if (vl.empty() || vr.empty()) return (vl.empty() ? ar : al).back();
        return op(al.back(), ar.back());
    }
};
