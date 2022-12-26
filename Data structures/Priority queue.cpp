//Analogue of std::priority_queue, but also has extract and reserve methods
//Performs the same as std::pq, but with reserve could be several percents faster
template<typename T, typename Compare = std::less<T>>
class my_priority_queue {
    int sz = 0;
    vector<T> store{T{}};

    void sift_up(size_t pos) {
        for (; pos > 1 && Compare{}(store[pos >> 1], store[pos]);) {
            swap(store[pos], store[pos >> 1]);
            pos >>= 1;
        }
    }

    void sift_down(size_t pos) {
        for (; pos * 2 <= sz;) {
            const size_t left = pos << 1;
            const size_t right = pos << 1 | 1;
            size_t swap_with = left;
            if (right <= sz && Compare{}(store[left], store[right])) {
                swap_with = right;
            }
            if (Compare{}(store[swap_with], store[pos])) {
                break;
            }
            swap(store[pos], store[swap_with]);
            pos = swap_with;
        }
    }

public:
    my_priority_queue() = default;
    my_priority_queue(size_t n): store(n + 1) {}

    size_t size() {return sz;}

    void push(T x) {
        if (++sz >= store.size()) store.resize(sz * 2);
        store[sz] = x;
        sift_up(sz);
    }

    T top() {
        assert(sz);
        return store[1];
    }

    void pop() {
        assert(sz);
        swap(store[1], store[sz--]);
        sift_down(1);
    }

    T extract_top() {
        assert(sz);
        T res = store[1];
        swap(store[1], store[sz--]);
        sift_down(1);
        return res;
    }
};