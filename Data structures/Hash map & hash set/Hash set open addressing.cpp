template<typename K>
class hashset_open_addressing {
	
	struct Iterator : public std::forward_iterator_tag {

		Iterator(size_t pos, const vector<K>& store, const vector<char>& is_occupied): pos_(pos), store_(store), is_occupied_(is_occupied) {
			while (pos_ < store_.size() && !is_occupied_[pos_]) ++pos_;
		}

		Iterator& operator++() {
			++pos_;
			while (pos_ < store_.size() && !is_occupied_[pos_]) ++pos_;
			return *this;
		}

		const K& operator*() const {
			return store_[pos_];
		}

		bool operator!=(const Iterator& rhs) const {
			return pos_ != rhs.pos_;
		}

		size_t pos_;
		const vector<K>& store_;
		const vector<char>& is_occupied_;
	};

	constexpr inline void next_pos(size_t& pos) const {
        pos = pos < capacity_ - 1 ? pos + 1 : 0;
    }

    static constexpr uint64_t kek = 11995408973635179863ull;
    constexpr inline size_t hsh(const K& key) const {
        if constexpr(is_integral_v<K>) {
            // return key & (capacity_ - 1);
            // return key % capacity_;
            return ((key * kek) >> 32) & (capacity_ - 1);
            // return ((key * kek) >> 32) % capacity_;
        } else {
            return hash<K>{}(key) % capacity_;
        }
    }

	static bool is_power_of_2(size_t x) {
		return (x & (x - 1)) == 0 && x;
	}

	void rehash(size_t new_capacity) {
		assert(size_ <= new_capacity);
    	assert(is_power_of_2(new_capacity));
		vector<K> content; content.reserve(size_);
		for (size_t i = 0; i < capacity_; ++i) {
			if (is_occupied_[i]) content.push_back(store_[i]);
		}
		clear();
		capacity_ = new_capacity;
		store_.resize(capacity_);
		is_occupied_.resize(capacity_);
		for (const K& key : content) insert(key);
	}

	size_t size_;
    size_t capacity_;
    vector<K> store_;
    vector<char> is_occupied_;
    double load_factor_ = 0.6;

public:
    hashset_open_addressing(size_t capacity = 1): size_(0) {
    	assert(capacity > 0);
    	capacity_ = is_power_of_2(capacity) ? capacity : 2 << __lg(capacity); 
    	store_.resize(capacity_);
    	is_occupied_.resize(capacity_);
    }

    Iterator begin() const {return Iterator(0, store_, is_occupied_);}
    Iterator end() const {return Iterator(store_.size(), store_, is_occupied_);}

    bool empty() const {return size_ == 0;}
    size_t size() const {return size_;}
    size_t capacity() const {return capacity_;}
    size_t count(const K& key) const {return contains(key);}

    void clear() {
        fill(is_occupied_.begin(), is_occupied_.end(), 0);
        size_ = 0;
    }

    bool contains(const K& key) const {
        size_t pos = hsh(key);
        while (is_occupied_[pos]) {
            if (store_[pos] == key) return true;
            next_pos(pos);
        }
        return false;
    }

    void insert(const K& key) {
        size_t pos = hsh(key);
        while (is_occupied_[pos]) {
            if (store_[pos] == key) return;
            next_pos(pos);
        }
        store_[pos] = key;
        is_occupied_[pos] = true;
        if (++size_ >= capacity_ * load_factor_) {
        	rehash(capacity_ * 2);
        }
    }

    void erase(const K& key) {
		size_t i = hsh(key);
		while (is_occupied_[i]) {
			if (store_[i] == key) break;
			next_pos(i);
		}
    	if (!is_occupied_[i]) return;
    	is_occupied_[i] = false; --size_;
    	size_t j = i;
    	for (next_pos(j); is_occupied_[j]; next_pos(j)) {
    		size_t k = hsh(store_[j]);
    		if (i < k || i > j && k <= j) continue;
    		swap(store_[i], store_[j]);
    		swap(is_occupied_[i], is_occupied_[j]);
    		i = j;
    	}
    }
};
