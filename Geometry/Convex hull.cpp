//Returns smallest subset of arr which generates convex hull, sorted in CCW order.
template<typename T>
vector<pt<T>> convex_hull(vector<pt<T>> arr) {
    pt<T> mnp = *min_element(arr.begin(), arr.end());
    for (auto& p : arr) p -= mnp;
    sort(arr.begin(), arr.end(), [&](const auto& p1, const auto& p2) {
        T c = cross(p1, p2);
        return c ? c > 0 : abs(p1.x) + abs(p1.y) < abs(p2.x) + abs(p2.y);
    });
    vector<pt<T>> ch;
    for (int i = 0; i < arr.size(); ++i) {
        for (; ch.size() > 1;) {
            if (cross(ch.back() - end(ch)[-2], arr[i] - ch.back()) > 0) break;
            ch.pop_back();
        }
        ch.push_back(arr[i]);
    }
    for (auto& p : ch) p += mnp;
    return ch;
}