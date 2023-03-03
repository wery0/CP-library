//This structure maintains a convex hull with the ability to add points to it.
template<typename T>
class incremental_convex_hull {

    vector<pt<T>> convex_hull(vector<pt<T>> arr) {
        if (arr.empty()) return {};
        pt<T> mnp = *min_element(arr.begin(), arr.end());
        for (auto& p : arr) p -= mnp;
        sort(arr.begin(), arr.end(), [&](const auto& p1, const auto& p2) {
            T c = cross(p1, p2);
            return c ? c > 0 : abs(p1.x) + abs(p1.y) < abs(p2.x) + abs(p2.y);
        });
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

public:
    vector<pt<T>> ch;

    incremental_convex_hull() = default;

    //O(log(|ch|)) for tangents + O(|ch|) in worst case for inserting/erasing subsegments
    //To minimize second term, add points in atan2 order
    void add_point(const pt<T>& p) {
        if (ch.size() < 3) {
            ch.push_back(p);
            ch = convex_hull(ch);
            return;
        }
        pair<int, int> location = point_location_convex_polygon(ch, p);
        if (location.first != -2) return;
        auto [l, r] = get_tangents_to_polygon(ch, p, location, true);
        if (l < r) {
            ch.erase(ch.begin() + l + 1, ch.begin() + r);
            ch.insert(ch.begin() + l + 1, p);
            if (l == 0 && (p.x < ch[0].x || (p.x == ch[0].x && p.y < ch[0].y))) {
                rotate(ch.begin(), ch.begin() + 1, ch.end());
            }
        } else if (l > r) {
            ch.erase(ch.begin() + l + 1, ch.end());
            ch.erase(ch.begin(), ch.begin() + r);
            if (r == 0) {
                if (p.x < ch[0].x || (p.x == ch[0].x && p.y < ch[0].y)) ch.insert(ch.begin(), p);
                else ch.push_back(p);
            } else ch.insert(ch.begin(), p);
        }
    }
};