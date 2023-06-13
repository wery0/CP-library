template<typename T>
class knapsack_weights_only {
    static_assert(is_unsigned_v<T>);

    struct item {T weight; size_t id;};

    vector<item> items;
    T weights_sum = 0;

    void transform_weights(vector<T>& weights) const {
        priority_queue<pair<T, size_t>, vector<pair<T, size_t>>, greater<pair<T, size_t>>> pq;
        sort(weights.begin(), weights.end());
        for (size_t l = 0; l < weights.size(); ++l) {
            size_t r = l;
            while (r + 1 < weights.size() && weights[r + 1] == weights[l]) ++r;
            pq.emplace(weights[l], r - l + 1);
            l = r;
        }
        weights.clear();
        while (pq.size()) {
            auto [wei, cnt] = pq.top(); pq.pop();
            while (pq.size() && pq.top().first == wei) cnt += pq.top().second, pq.pop();
            if (cnt < 3) {
                for (size_t i = 0; i < cnt; ++i) weights.push_back(wei);
            } else {
                size_t c = cnt + 1;
                for (T nwei = wei; c > 1; c >>= 1, nwei <<= 1) {
                    pq.emplace(nwei, (c & 1) + 1);
                }
            }
        }
    }

    void transform_weights_ids(vector<pair<T, vector<size_t>>>& kek) const {
        priority_queue<pair<T, vector<size_t>>, vector<pair<T, vector<size_t>>>, greater<pair<T, vector<size_t>>>> pq;
        for (const auto& [wei, nums] : kek) pq.emplace(wei, nums);
        kek.clear();
        while (pq.size()) {
            T wei = pq.top().first;
            vector<vector<size_t>> store;
            while (pq.size() && pq.top().first == wei) store.push_back(pq.top().second), pq.pop();
            if (store.size() < 3) {
                for (const auto& nums : store) kek.emplace_back(wei, nums);
            } else {
                size_t c = store.size() + 1;
                for (T nwei = wei, mul = 1; c > 1; c >>= 1, nwei <<= 1, mul <<= 1) {
                    for (size_t i = 0; i < (c & 1) + 1; ++i) {
                        vector<size_t> nums;
                        for (size_t j = 0; j < mul; ++j) {
                            nums.insert(nums.end(), store.back().begin(), store.back().end());
                            store.pop_back();
                        }
                        pq.emplace(nwei, nums);
                    }
                }
            }
        }
    }

public:
    knapsack_weights_only() = default;
    template<typename I>
    knapsack_weights_only(I first, I last) {
        const size_t n = last - first;
        items.resize(n);
        weights_sum = accumulate(first, last, (T)0);
        for (size_t i = 0; i < n; ++i, ++first) {
            items[i].weight = *first;
            items[i].id = i;
        }
    }
    template<typename T_arr>
    knapsack_weights_only(const T_arr& arr) {
        *this = knapsack_weights_only<T>(arr.begin(), arr.end());
    }

    T get_weights_sum() const {return weights_sum;}

    void clear() {items.clear(); weights_sum = 0;}

    void insert(T weight, size_t id = numeric_limits<size_t>::max()) {
        if (id == numeric_limits<size_t>::max()) id = items.size();
        items.emplace_back(weight, id); weights_sum += weight;
    }

    //With 3k trick: <O(min(N, sqrt(weights_sum)) * MAX_SUM / W), O(MAX_SUM / W)>
    //Without trick: <O(N * MAX_SUM / W), O(MAX_SUM / W)>
    template<const T MAX_SUM>
    bool contains_subset_with_fixed_sum_bitset(T sum, bool apply_3k_trick = true) const {
        if (sum > weights_sum) return false;
        sum = min(sum, weights_sum - sum);
        assert(sum <= MAX_SUM && "too small bitset");
        bitset<MAX_SUM + 1> bs;
        bs[0] = 1;
        vector<T> weights(items.size());
        for (size_t i = 0; i < items.size(); ++i) weights[i] = items[i].weight;
        if (apply_3k_trick) transform_weights(weights);
        shuffle(weights.begin(), weights.end(), mt19937());
        for (const T& wei : weights) {
            if (wei == 0 || wei > sum) continue;
            bs |= bs << wei;
            if (bs[sum]) return true;
        }
        return bs[sum];
    }

    //X = # different subset sums of size N / 2; X <= sum <= O(2^(N/2))
    //<O(X), O(X)>
    bool contains_subset_with_fixed_sum_MITM(T sum, bool apply_3k_trick = true) const {
        if (sum > weights_sum) return false;
        sum = min(sum, weights_sum - sum);
        vector<T> weights(items.size());
        for (size_t i = 0; i < items.size(); ++i) weights[i] = items[i].weight;
        if (apply_3k_trick) transform_weights(weights);
        shuffle(weights.begin(), weights.end(), mt19937());
        const size_t L = weights.size() / 2, R = weights.size() - L;
        vector<T> lf(weights.begin(), weights.begin() + L);
        vector<T> rg(weights.begin() + L, weights.end());
        unordered_set<T> sl = {0};
        unordered_set<T> tmp = sl;
        for (const T x : lf) {
            for (const T c : sl) if (c + x <= sum) tmp.insert(c + x);
            sl = tmp;
            if (sl.count(sum)) return true;
        }
        unordered_set<T> sr = {0};
        tmp = sr;
        for (const T x : rg) {
            for (const T c : sr) if (c + x <= sum) tmp.insert(c + x);
            sr = tmp;
            if (sr.count(sum)) return true;
        }
        for (const T s : sr) if (sl.count(sum - s)) return true;
        return false;
    }

    //With 3k trick: O(min(N, sqrt(weights_sum)) * MAX_SUM / W) time and memory
    //Without trick: O(N * MAX_SUM / W) time and memory
    //Returns vector<pair<weight, id>> s. t. accumulate(all(weight)) == sum
    template<const T MAX_SUM>
    pair<vector<T>, vector<size_t>> get_subset_with_fixed_sum_bitset(T sum, bool apply_3k_trick = true) const {
        if (sum > weights_sum) return {};
        bool is_inversed = false;
        if (weights_sum - sum < sum) {
            sum = weights_sum - sum;
            is_inversed = true;
        }
        assert(sum <= MAX_SUM && "too small bitset");
        vector<pair<T, vector<size_t>>> kek(items.size());
        for (size_t i = 0; i < items.size(); ++i) kek[i] = {items[i].weight, {i}};
        if (apply_3k_trick) transform_weights_ids(kek);
        shuffle(kek.begin(), kek.end(), mt19937());
        vector<bitset<MAX_SUM + 1>> store(1, bitset<MAX_SUM + 1>());
        store[0][0] = 1;
        for (const auto& [wei, ids] : kek) {
            if (store.back()[sum]) break;
            store.push_back(store.back() | (store.back() << wei));
        }
        if (!store.back()[sum]) return {};    //No subset with needed sum
        T eso = sum;
        vector<T> ans_weigths;
        vector<size_t> ans_ids;
        for (size_t i = store.size() - 1; i; --i) {
            if (!(is_inversed ^ store[i - 1][eso])) {
                auto [wei, ids] = kek[i - 1];
                for (size_t i : ids) {
                    auto [wei, id] = items[i];
                    ans_weigths.push_back(wei);
                    ans_ids.push_back(id);
                }
            }
            if (!store[i - 1][eso]) eso -= kek[i - 1].first;
        }
        if (is_inversed) {
            for (size_t i = store.size(); i <= kek.size(); ++i) {
                auto [wei, ids] = kek[i - 1];
                for (size_t i : ids) {
                    auto [wei, id] = items[i];
                    ans_weigths.push_back(wei);
                    ans_ids.push_back(id);
                }
            }
        }
        return {ans_weigths, ans_ids};
    }

    //With 3k trick: <O(min(N, sqrt(weights_sum)) * MAX_SUM / W), O(MAX_SUM / W)>
    //Without trick: <O(N * MAX_SUM / W), O(MAX_SUM / W)>
    template<const T MAX_SUM>
    vector<T> get_possible_subset_sums_bitset(bool apply_3k_trick = true) const {
        bitset<MAX_SUM + 1> sums;
        sums[0] = 1;
        vector<T> weights;
        for (const auto& [wei, id] : items) {
            if (wei <= MAX_SUM) weights.push_back(wei);
        }
        if (apply_3k_trick) transform_weights(weights);
        for (const T& wei : weights) {
            if (wei <= MAX_SUM) sums |= sums << wei;
        }
        vector<T> ans; ans.reserve(sums.count());
        for (size_t i = 0; i <= MAX_SUM; i = sums._Find_next(i)) {
            ans.push_back(i);
        }
        return ans;
    }

    //Returns dp of size MAX_SUM + 1, s. t. dp[k] = min # items s. t. their sum is k, or -1 if it is impossible
    //<O(N * min(weigths_sum, MAX_SUM)), O(N + MAX_SUM)>
    vector<ssize_t> get_minimum_possible_items(const T MAX_SUM) {
        const size_t INF = items.size() + 1;
        vector<T> weights;
        for (const auto& [wei, id] : items) {
            if (0 < wei && wei <= MAX_SUM) weights.push_back(wei);
        }
        sort(weights.begin(), weights.end());
        vector<ssize_t> dp(MAX_SUM + 1, INF);
        dp[0] = 0;
        for (T sum = 0; const auto& wei : weights) {
            sum += wei;
            for (T i = min(sum, MAX_SUM); i >= wei; --i) {
                dp[i] = min(dp[i], dp[i - wei] + 1);
            }
        }
        for (auto& i : dp) if (i == INF) i = -1;
        return dp;
    }

    //Returns dp of size MAX_SUM + 1, s. t. dp[k] = # subsets with sum k.
    //<O(N * min(weigths_sum, MAX_SUM)), O(N + MAX_SUM)>
    template<typename C>
    vector<C> get_subset_sum_counts(const T MAX_SUM) {
        vector<T> weights;
        C pw2 = 1;
        for (const auto& [wei, id] : items) {
            if (wei == 0) pw2 *= 2;
            else if (wei <= MAX_SUM) weights.push_back(wei);
        }
        sort(weights.begin(), weights.end());
        vector<C> dp(MAX_SUM + 1);
        dp[0] = 1;
        for (T sum = 0; const auto& wei : weights) {
            sum += wei;
            for (T i = min(sum, MAX_SUM); i >= wei; --i) {
                dp[i] += dp[i - wei];
            }
        }
        for (C& c : dp) c *= pw2;
        return dp;
    }
};