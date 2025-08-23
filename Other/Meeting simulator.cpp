/*
Simulates arrangement of meetings in rooms.
Each meeting is defined by [start_time, duration].
Each meeting will be arranged in the room with the smallest number among free rooms
at the moment max(start_time, (when the first free room appears)) and will last duration time units.
This function for every meeting calculates its starting time and room, where it will be.
ALL start_time SHOULD BE DIFFERENT FOR UNIQUE ANSWER!
*/
//https://leetcode.com/problems/meeting-rooms-iii/
template<typename T>
vector<pair<T, T>> meeting_simulator(T rooms, vector<pair<T, T>> meetings) {
    //rooms = min(rooms, (T)meetings.size());           //<- uncomment if it may help
    const size_t n = meetings.size();
    vector<size_t> nums(n);
    iota(nums.begin(), nums.end(), (size_t)0);
    sort(nums.begin(), nums.end(), [&meetings](const auto& l, const auto& r) {
        return meetings[l].first < meetings[r].first;
    });
    priority_queue<pair<T, T>, vector<pair<T, T>>, greater<pair<T, T>>> pq;
    priority_queue<T, vector<T>, greater<T>> free;
    for (size_t i = 0; i < rooms; ++i) free.push(i);
    T cur_t = 0;
    vector<pair<T, T>> when_where(n);
    for (size_t i = 0; i < n; ++i) {
        auto [start_time, duration] = meetings[nums[i]];
        cur_t = max(cur_t, start_time);
        if (free.empty()) cur_t = max(cur_t, pq.top().first);
        while (pq.size() && pq.top().first <= cur_t) {
            free.push(pq.top().second);
            pq.pop();
        }
        //i-th meeting will be in room room_num
        T room_num = free.top(); free.pop();
        pq.emplace(cur_t + duration, room_num);
        when_where[nums[i]] = {cur_t, room_num};
    }
    return when_where;
}
