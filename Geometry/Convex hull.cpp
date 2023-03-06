//Returns smallest subset of arr which generates convex hull, sorted in CCW order.
template<typename T>
vector<pt<T>> convex_hull(vector<pt<T>> arr) {
    if(arr.empty()) return {};
    pt<T> mnp = *min_element(arr.begin(), arr.end());
    for (auto& p : arr) p -= mnp;
    sort(arr.begin(), arr.end(), [&](const auto& p1, const auto& p2) {
        T c = cross(p1, p2);
        return c ? c > 0 : abs(p1.x) + abs(p1.y) < abs(p2.x) + abs(p2.y);
    });
    if (arr[0] == arr.back()) return {arr[0]};
    vector<pt<T>> ch;
    for (const auto& p : arr) {
        while (ch.size() > 1) {
            if (cross(ch.back() - ch[ch.size() - 2], p - ch.back()) > 0) break;
            ch.pop_back();
        }
        ch.push_back(p);
    }
    for (auto& p : ch) p += mnp;
    return ch;
}