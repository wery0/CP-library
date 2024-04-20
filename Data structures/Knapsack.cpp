template<typename T>
class knapsack {

    struct item {T weight, cost; size_t id;};

    static constexpr T INF = numeric_limits<T>::max() / 2;
    vector<item> items;
    T weights_sum = 0;
    T costs_sum = 0;

public:
    knapsack() = default;

    template<typename I1, typename I2>
    knapsack(I1 firstw, I1 lastw, I2 firstc, I2 lastc) {
        const size_t n = lastw - firstw;
        assert(lastc - firstc == n);
        items.resize(n);
        weights_sum = accumulate(firstw, lastw, (T)0);
        costs_sum = accumulate(firstc, lastc, (T)0);
        assert(all_of(firstw, lastw, [](const auto& i) {return i >= 0;}));
        assert(all_of(firstc, lastc, [](const auto& i) {return i >= 0;}));
        for (size_t i = 0; i < n; ++i, ++firstw, ++firstc) {
            items[i].weight = *firstw;
            items[i].cost = *firstc;
            items[i].id = i;
        }
    }

    template<typename T_arr1, typename T_arr2>
    knapsack(const T_arr1& weights, const T_arr2& costs) {
        *this = knapsack<T>(weights.begin(), weights.end(), costs.begin(), costs.end());
    }

    template<typename U>
    knapsack(const vector<pair<U, U>>& items) {
        vector<T> weights(items.size());
        vector<T> costs(items.size());
        for (size_t i = 0; i < items.size(); ++i) {
            weights[i] = items[i].first;
            costs[i] = items[i].second;
        }
        *this = knapsack<T>(weights.begin(), weights.end(), costs.begin(), costs.end());
    }

    T get_weights_sum() const {return weights_sum;}
    T get_costs_sum() const {return costs_sum;}

    void clear() {items.clear(); weights_sum = costs_sum = 0;}

    void insert(T weight, T cost, size_t id = numeric_limits<size_t>::max()) {
        assert(weight >= 0);
        assert(cost >= 0);
        if (id == numeric_limits<size_t>::max()) id = items.size();
        items.emplace_back(weight, cost, id);
        weights_sum += weight;
        costs_sum += cost;
    }

    //Returns dp of size MAX_W + 1, s. t. dp[k] = max cost of knapsack exactly weight k, or 0 if it's impossible to get exactly k.
    //<O(MAX_W * N), O(N + MAX_W)>
    vector<T> calc_max_cost(const T MAX_W) {
        vector<pair<T, T>> store;
        vector<T> dp(MAX_W + 1);
        for (const auto& [w, c, id] : items) {
            if (w == 0) dp[0] += c;
            else if (w <= MAX_W) store.emplace_back(w, c);
        }
        sort(store.begin(), store.end());
        for (T smw = 0; auto [w, c] : store) {
            smw += w;
            for (T cur_w = min(MAX_W, smw); cur_w >= w; --cur_w) {
                if (dp[cur_w - w] || cur_w == w) dp[cur_w] = max(dp[cur_w], dp[cur_w - w] + c);
            }
        }
        return dp;
    }

    //Returns (max_cost, vector of id's of items s. t. their weight <= MAX_W, their cost == max_cost)
    //<O(MAX_W * N), O(MAX_W * N)>
    pair<T, vector<size_t>> calc_max_cost_answer_recovery(const T MAX_W) {
        vector<array<T, 3>> store;
        vector<T> dp(MAX_W + 1);
        vector<size_t> res;
        for (const auto& [w, c, id] : items) {
            if (w == 0) dp[0] += c, res.push_back(id);
            else if (c > 0 && w <= MAX_W) store.push_back({w, c, T(id)});
        }
        sort(store.begin(), store.end());
        vector who(store.size(), vector<int>(MAX_W + 1, -1));
        for (T i = 0, smw = 0; i < store.size(); ++i) {
            auto [w, c, id] = store[i];
            smw += w;
            for (T cur_w = min(MAX_W, smw); cur_w >= w; --cur_w) {
                if ((dp[cur_w - w] || w == cur_w) && dp[cur_w - w] + c > dp[cur_w]) {
                    dp[cur_w] = dp[cur_w - w] + c;
                    who[i][cur_w] = i;
                }
            }
        }
        T w = max_element(dp.begin(), dp.end()) - dp.begin(), max_cost = dp[w];
        for (size_t i = store.size(); w != 0; --i) {
            assert(i);
            if (who[i - 1][w] == -1) continue;
            const auto& item = store[who[i - 1][w]];
            if (item[2] != -1) {
                res.push_back(item[2]);
                w -= item[0];
            }
        }
        return {max_cost, res};
    }

    //Returns dp of size MAX_C + 1, s. t. dp[k] = min weigth of knapsack exactly cost k, or INF if it's impossible to get exactly k.
    //<O(MAX_C * N), O(N + MAX_C)>
    vector<T> calc_min_weight(const T MAX_C) {
        vector<pair<T, T>> store;
        for (const auto& [w, c, id] : items) {
            if (c && c <= MAX_C) store.emplace_back(c, w);
        }
        sort(store.begin(), store.end());
        vector<T> dp(MAX_C + 1, INF);
        dp[0] = 0;
        for (T smc = 0; auto [c, w] : store) {
            smc += c;
            for (T cur_c = min(MAX_C, smc); cur_c >= c; --cur_c) {
                dp[cur_c] = min(dp[cur_c], dp[cur_c - c] + w);
            }
        }
        return dp;
    }

    //Returns max cost of knapsack size <= MAX_W
    //Something between O(N) and O(2^N) :D (but fast on practice)
    T branch_and_bound_max_cost(T MAX_W) {
        vector<pair<T, T>> store;
        T free = 0;
        for (const auto& [w, c, id] : items) {
            if (w == 0) free += c;
            else if (c && w <= MAX_W) store.emplace_back(w, c);
        }
        sort(store.begin(), store.end(), [](const pair<T, T>& l, const pair<T, T>& r) {
            auto [w1, c1] = l;
            auto [w2, c2] = r;
            return c1 * w2 > c2 * w1;
        });
        vector<T> psw(store.size());
        vector<T> psc(store.size());
        for (size_t i = 0; i < store.size(); ++i) {
            psw[i] = store[i].first;
            psc[i] = store[i].second;
        }
        partial_sum(psw.begin(), psw.end(), psw.begin());
        partial_sum(psc.begin(), psc.end(), psc.begin());
        auto ub_profit = [&](size_t i, T cur_w) {
            if (i >= store.size() || cur_w > MAX_W) return (T)0;
            T rem_w = MAX_W - cur_w;

            //O(log(n))
            T ls = i ? psw[i - 1] : 0;
            size_t r = lower_bound(psw.begin(), psw.end(), ls + rem_w) - psw.begin();
            if (r == psw.size()) --r;
            T df = psw[r] - ls;
            T smc = psc[r] - (i ? psc[i - 1] : 0);
            if (df > rem_w) {
                smc -= 1.0 * (df - rem_w) / store[r].first * store[r].second;
            }
            return smc;

            //O(n) in worst case
            // T o = 0;
            // for (size_t j = i; j < store.size(); ++j) {
            //     auto [w, c] = store[j];
            //     if (w <= rem_w) rem_w -= w, o += c;
            //     else {
            //         o += 1.0 * rem_w / w * c;
            //         break;
            //     }
            // }
            // return o;
        };
        deque<item> kek;
        kek.emplace_back(0, 0, 0);
        T max_cost = 0, p1 = -1;
        while (kek.size()) {
            auto [w, c, i] = kek.front(); kek.pop_front();
            if (i == store.size()) continue;
            auto& [sw, sc] = store[i];
            T np1 = p1 == -1 ? ub_profit(i, w) : p1;
            if (c + np1 > max_cost) {
                if (w + sw <= MAX_W) {
                    kek.emplace_front(w + sw, c + sc, i + 1);
                    p1 = np1;
                    max_cost = max(max_cost, c + sc);
                } else p1 = -1;
            }
            //if (c + np1 > max_cost) kek.emplace_back(w, c, i + 1);    //Faster, but more states
            if (c + ub_profit(i + 1, w) > max_cost) kek.emplace_back(w, c, i + 1);  //Slower, but less states
        }
        return free + max_cost;
    }
};
