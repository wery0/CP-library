//Some problems might have non-trivial reduction to this exact problem.
//For example: given n bracket sequences (not necessary right), permute them to make right bracket sequence if possible.
//Solution: transform bracket sequence of depth d and min prefix depth mnd to the task {-d, mnd - d}
template<typename T>
class TaskScheduler {
    using task = array<T, 3>;
    vector<task> store;

public:
    TaskScheduler() = default;

    void clear() {store.clear();}

    //Both parameters can be any numbers including <= 0
    void add_task(T time_to_do_task, T last_time_to_finish_task, T num = numeric_limits<T>::max()) {
        store.push_back({last_time_to_finish_task, time_to_do_task, num == numeric_limits<T>::max() ? T(store.size()) : num});
    }

    //Finds maximum number of scheduable tasks and returns order in which they can be done
    //Assumes that starting time is 0
    //O(nlogn)
    vector<T> schedule_max_tasks() const {
        T cur_t = 0;
        auto cmp = [&](const task& l, const task& r) {return l[0] - l[1] < r[0] - r[1];};
        priority_queue<task, vector<task>, decltype(cmp)> pq(cmp);
        auto tasks = store;
        for (auto [lttf, ttd, n] : tasks) {
            if (ttd <= 0) pq.push({lttf, ttd, n});
        }
        vector<T> order;
        while (pq.size()) {
            auto [lttf, ttd, n] = pq.top();
            pq.pop();
            if (cur_t <= lttf - ttd) cur_t += ttd, order.push_back(n);
            else break;
        }
        tasks.erase(remove_if(tasks.begin(), tasks.end(), [cur_t](const task& t) {return t[0] <= cur_t || t[1] <= 0;}), tasks.end());
        sort(tasks.begin(), tasks.end(), [](const task& l, const task& r) {return l[0] > r[0];});
        tasks.push_back({cur_t, 1, -1});
        priority_queue<task, vector<task>, greater<task>> qq;
        vector<pair<T, T>> who;
        for (T pr = tasks[0][0]; auto [lttf, ttd, n] : tasks) {
            T len = pr - lttf;
            while (len && qq.size()) {
                auto [eso, lttf, n] = qq.top(); qq.pop();
                T mn = min(len, eso);
                len -= mn, eso -= mn;
                if (eso) qq.push({eso, lttf, n});
                else who.push_back({lttf, n});
            }
            qq.push({ttd, lttf, n});
            pr = lttf;
        }
        sort(who.begin(), who.end(), [&](const pair<T, T>& l, const pair<T, T>& r) {return l < r;});
        for (auto [lttf, n] : who) order.push_back(n);
        return order;
    }
};
