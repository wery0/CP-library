//This class efficiently works with checkered figures
template<typename T>
class checkered_figure {
    static constexpr int dx[] = {-1, 0, 1, 0, -1, 1, 1, -1};
    static constexpr int dy[] = {0, 1, 0, -1, 1, 1, -1, -1};
    static const int DIRECTIONS = 4;

    size_t perimeter = 0;
    mutable bool _is_connected = 1, is_certain = 1;
    set<pair<T, T>> mp;

public:
    checkered_figure() = default;

    size_t size() const {return mp.size();}
    size_t get_perimeter() const {return perimeter;}

    bool is_connected() const {
        if (is_certain) return _is_connected;
        if (mp.empty()) return true;
        set<pair<T, T>> us = {*mp.begin()};
        deque<pair<T, T>> dq = {*mp.begin()};
        while (dq.size()) {
            auto [x, y] = dq[0]; dq.pop_front();
            for (int d = 0; d < DIRECTIONS; ++d) {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (mp.count({nx, ny}) && !us.count({nx, ny})) {
                    us.insert({nx, ny});
                    dq.emplace_back(nx, ny);
                }
            }
        }
        _is_connected = us.size() == mp.size();
        is_certain = 1;
        return _is_connected;
    }

    void insert(int x, int y) {
        if (mp.count({x, y})) return;
        mp.insert({x, y});
        for (int d = 0; d < DIRECTIONS; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (mp.count({nx, ny})) --perimeter;
            else ++perimeter;
        }
        if (is_certain && !_is_connected) is_certain = 0;
    }

    void erase(int x, int y) {
        if (!mp.count({x, y})) return;
        mp.erase({x, y});
        for (int d = 0; d < DIRECTIONS; ++d) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (mp.count({nx, ny})) ++perimeter;
            else --perimeter;
        }
        if (is_certain && _is_connected) is_certain = 0;
    }

    bool contains(int x, int y) const {return mp.count({x, y});}
    bool count(int x, int y) const {return mp.count({x, y});}

    set<pair<T, T>> get_adjacent() const {
        if (mp.empty()) return {{0, 0}};
        set<pair<T, T>> res;
        for (auto [x, y] : mp) {
            for (int d = 0; d < DIRECTIONS; ++d) {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (!mp.count({nx, ny})) {
                    res.insert({nx, ny});
                }
            }
        }
        return res;
    }

    //Returns cells that can be removed without increasing number of connected components (i. e. all non-cutpoint cells)
    set<pair<T, T>> get_removable() const {
        set<pair<T, T>> us;
        map<pair<T, T>, int> ans;
        map<pair<T, T>, int> tin;
        map<pair<T, T>, int> fup;
        for (auto v : mp) ans[v] = mp.size() > 1;
        auto dfs = [&](auto&& dfs, pair<T, T> v, pair<T, T> p) -> void {
            static int t = 0;
            auto [x, y] = v;
            us.insert(v);
            tin[v] = fup[v] = t++;
            int chd = 0;
            for (int d = 0; d < DIRECTIONS; ++d) {
                int nx = x + dx[d];
                int ny = y + dy[d];
                pair<T, T> h = {nx, ny};
                if (!mp.count(h)) continue;
                if (h == p) continue;
                if (!us.count(h)) {
                    ++chd;
                    dfs(dfs, h, v);
                    fup[v] = min(fup[v], fup[h]);
                    ans[v] += p == v ? chd > 1 : fup[h] >= tin[v];
                } else {
                    fup[v] = min(fup[v], tin[h]);
                }
            }
        };
        for (auto v : mp) {
            if (!us.count(v)) dfs(dfs, v, v);
        }
        set<pair<T, T>> res;
        for (auto [v, c] : ans) if (c <= 1) res.insert(v);
        return res;
    }
};
