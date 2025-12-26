template<typename T, typename C>
class offline_shortest_path_queries_on_grid {
    static constexpr C inf = numeric_limits<C>::max() / 2;
    static constexpr array<int, 4> dx = {-1, 0, 1, 0};
    static constexpr array<int, 4> dy = {0, 1, 0, -1};

    size_t n, m;
    vector<vector<array<T, 4>>> dd;
    vector<array<int, 4>> queries;

public:
    offline_shortest_path_queries_on_grid() = default;
    //dd[x][y][d] is the distance from (x, y) to (x + dx[d], y + dy[d])
    offline_shortest_path_queries_on_grid(const vector<vector<array<T, 4>>>& dd, size_t q = 0): n(dd.size()), m(n ? dd[0].size() : 0), dd(dd) {
        if (q) queries.reserve(q);
    }

    void clear() {queries.clear();}
    void reserve(size_t q) {queries.reserve(q);}

    void add_query(size_t x1, size_t y1, size_t x2, size_t y2) {
        assert(max(x1, x2) < n);
        assert(max(y1, y2) < m);
        queries.push_back({int(x1), int(y1), int(x2), int(y2)});
    }

    //For a square grid n * n time complexity is ~ (2 + sqrt(2))n * T(n ^ 2) + 2nq = O(n * T(n ^ 2) + nq) where T(V) is time complexity of SSSP algorithm on a grid of size V. Memory complexity is O(nm).
    vector<C> solve() {
        vector dst(n, vector<C>(m, inf));
        vector<C> o(queries.size(), inf);
        auto cmp = [&](const array<C, 3>& l, const array<C, 3>& r) {return l[0] > r[0];};
        priority_queue<array<C, 3>, vector<array<C, 3>>, decltype(cmp)> pq(cmp);
        deque<array<int, 2>> que;
        vector inq(n, vector<char>(m));
        auto SSSP = [&](int x, int y, int lx, int ly, int rx, int ry) {
            assert(lx <= x && x <= rx);
            assert(ly <= y && y <= ry);
            for (int i = lx; i <= rx; ++i) fill(dst[i].begin() + ly, dst[i].begin() + ry + 1, inf);
            dst[x][y] = 0;

            auto dijkstra = [&]() {
                pq.push({0, x, y});
                while (pq.size()) {
                    auto [ds, x, y] = pq.top(); pq.pop();
                    if (ds != dst[x][y]) continue;
                    for (int d = 0; d < 4; ++d) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (nx < lx || ny < ly || nx > rx || ny > ry) continue;
                        if (C nd = ds + dd[x][y][d]; nd < dst[nx][ny]) {
                            dst[nx][ny] = nd;
                            pq.push({nd, nx, ny});
                        }
                    }
                }
            };

            auto spfa = [&]() {
                que = {{x, y}};
                inq[x][y] = 1;
                while (!que.empty()) {
                    constexpr size_t MAGIC = 2;
                    for (size_t i = 0; i < MAGIC && i + 1 < que.size(); ++i) {
                        if (dst[que.back()[0]][que.back()[1]] < dst[que[i][0]][que[i][1]]) {
                            swap(que.back(), que[i]);
                            break;
                        }
                    }
                    auto [x, y] = que.front(); que.pop_front();
                    inq[x][y] = 0;
                    for (int d = 0; d < 4; ++d) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (nx < lx || ny < ly || nx > rx || ny > ry) continue;
                        if (C nd = dst[x][y] + dd[x][y][d]; nd < dst[nx][ny]) {
                            dst[nx][ny] = nd;
                            if (!inq[nx][ny]) {
                                inq[nx][ny] = 1;
                                que.push_back({nx, ny});
                            }
                        }
                    }
                }
            };

            // dijkstra();
            spfa();
        };

        function<void(int, int, int, int, vector<int>&)> go = [&](int lx, int ly, int rx, int ry, vector<int>& nums) {
            if (nums.empty()) return;
            vector<pair<int, int>> who(min(rx - lx, ry - ly) + 1);
            int mx = lx + (rx - lx) / 2;
            int my = ly + (ry - ly) / 2;
            int isy = ry - ly < rx - lx;
            if (isy) for (int y = ly; y <= ry; ++y) who[y - ly] = {mx, y};
            else for (int x = lx; x <= rx; ++x) who[x - lx] = {x, my};
            if (nums.size() < who.size()) {
                for (int n : nums) {
                    auto [x1, y1, x2, y2] = queries[n];
                    SSSP(x1, y1, lx, ly, rx, ry);
                    o[n] = min(o[n], dst[x2][y2]);
                }
                return;
            }
            vector<int> lf, rg;
            for (int n : nums) {
                auto [x1, y1, x2, y2] = queries[n];
                if (isy) {
                    if (x1 < mx && x2 < mx) lf.push_back(n);
                    else if (x1 > mx && x2 > mx) rg.push_back(n);
                } else {
                    if (y1 < my && y2 < my) lf.push_back(n);
                    else if (y1 > my && y2 > my) rg.push_back(n);
                }
            }

            for (auto [x, y] : who) {
                SSSP(x, y, lx, ly, rx, ry);
                for (int n : nums) {
                    auto [x1, y1, x2, y2] = queries[n];
                    o[n] = min(o[n], dst[x1][y1] + dst[x2][y2]);
                }
            }
            if (isy) {
                go(lx, ly, mx - 1, ry, lf);
                go(mx + 1, ly, rx, ry, rg);
            } else {
                go(lx, ly, rx, my - 1, lf);
                go(lx, my + 1, rx, ry, rg);
            }
        };
        function<void(int, int, int, int, vector<int>&)> go2 = [&](int lx, int ly, int rx, int ry, vector<int>& nums) {
            if (nums.empty()) return;
            vector<pair<int, int>> who;
            int mx = lx + (rx - lx) / 2;
            int my = ly + (ry - ly) / 2;
            for (int y = ly; y <= ry; ++y) who.emplace_back(mx, y);
            for (int x = lx; x <= rx; ++x) if (x != mx) who.emplace_back(x, my);
            if (nums.size() < who.size()) {
                for (int n : nums) {
                    auto [x1, y1, x2, y2] = queries[n];
                    SSSP(x1, y1, lx, ly, rx, ry);
                    o[n] = min(o[n], dst[x2][y2]);
                }
                return;
            }
            vector<int> mm, mb, bm, bb;
            for (int n : nums) {
                auto [x1, y1, x2, y2] = queries[n];
                if (x1 < mx && y1 < my && x2 < mx && y2 < my) mm.push_back(n);
                if (x1 < mx && y1 > my && x2 < mx && y2 > my) mb.push_back(n);
                if (x1 > mx && y1 < my && x2 > mx && y2 < my) bm.push_back(n);
                if (x1 > mx && y1 > my && x2 > mx && y2 > my) bb.push_back(n);
            }

            for (auto [x, y] : who) {
                SSSP(x, y, lx, ly, rx, ry);
                for (int n : nums) {
                    auto [x1, y1, x2, y2] = queries[n];
                    o[n] = min(o[n], dst[x1][y1] + dst[x2][y2]);
                }
            }
            go2(lx, ly, mx - 1, my - 1, mm);
            go2(lx, my + 1, mx - 1, ry, mb);
            go2(mx + 1, ly, rx, my - 1, bm);
            go2(mx + 1, my + 1, rx, ry, bb);
        };
        vector<int> nums(queries.size()); iota(nums.begin(), nums.end(), 0);
        go(0, 0, n - 1, m - 1, nums);
        // go2(0, 0, n - 1, m - 1, nums);
        return o;
    }
};
