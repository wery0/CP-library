//Analogue of std::priority_queue, but also has extract and reserve methods
//Performs the same as std::pq, but with reserve could be several percents faster
template<typename T, typename Compare = std::less<T>>
class my_priority_queue {
    size_t N = 0;
    vector<T> store{T{}};

    void sift_up(size_t pos) {
        while (pos > 1 && Compare{}(store[pos >> 1], store[pos])) {
            swap(store[pos], store[pos >> 1]);
            pos >>= 1;
        }
    }

    void sift_down(size_t pos) {
        while (pos * 2 <= N) {
            const size_t left = pos << 1;
            const size_t right = pos << 1 | 1;
            size_t swap_with = left;
            if (right <= N && Compare{}(store[left], store[right])) {
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

    size_t size() const {return N;}
    bool empty() const {return N == 0;}

    void clear() {N = 0;}

    void push(T x) {
        if (++N >= store.size()) store.resize(N * 2);
        store[N] = x;
        sift_up(N);
    }

    T top() const {
        assert(N);
        return store[1];
    }

    void pop() {
        assert(N);
        swap(store[1], store[N--]);
        sift_down(1);
    }

    T extract_top() {
        assert(N);
        T res = store[1];
        swap(store[1], store[N--]);
        sift_down(1);
        return res;
    }
};
