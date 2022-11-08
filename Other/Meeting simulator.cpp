//Simulates arrangement of meetings in rooms.
//Each meeting is defined by [start_time, duration]
/*Each meeting wiil be arranged in the room with the smallest number among free rooms
at the moment max(start_time, (when the first free room appears)) and will lasts duration time units*/
//This function for every meeting calculates the room, where it will be.
template<typename T>
vec<int> meeting_simulator(T rooms, vec<pair<T, T>> meetings) {
    //chmin(rooms, meetings.size());           //<- uncomment if it may help
    sort(all(meetings), [&](const auto & l, const auto & r) {
        return l.F < r.F;
    });
    priority_queue<pair<ll, T>, vec<pair<ll, T>>, greater<pair<ll, T>>> pq;
    priority_queue<int, vec<int>, greater<int>> free;
    for (int q = 0; q < rooms; ++q) free.push(q);
    ll cur_t = 0;
    vec<int> where(meetings.size());
    for (int i = 0; i < meetings.size(); ++i) {
        auto [start_time, duration] = meetings[i];
        chmax(cur_t, start_time);
        if (free.empty()) chmax(cur_t, pq.top().F);
        for (; pq.size() && pq.top().F <= cur_t;) {
            free.push(pq.top().S);
            pq.pop();
        }
        //i-th meeting will be in room room_num
        int room_num = free.top(); free.pop();
        pq.push({cur_t + duration, room_num});
        where[i] = room_num;
    }
    return where;
}
//Can test here: https://leetcode.com/problems/meeting-rooms-iii/