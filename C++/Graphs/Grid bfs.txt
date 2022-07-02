namespace grid_bfs {

    void go(vec<vec<int>> &m, const pii st, vec<vec<int>> &dst, const int inf = numeric_limits<int>::max()) {
        const int a = m.size();
        const int b = m[0].size();
        dst[st.F][st.S] = 0;
        deque<pii> dq = {st};
        const int dx[] = { -1, 0, 1, 0};
        const int dy[] = {0, 1, 0, -1};
        while (!dq.empty()) {
            auto [x, y] = dq.front(); dq.pop_front();
            for (int q = 0; q < 4; ++q) {
                int nx = x + dx[q];
                int ny = y + dy[q];
                if (nx < 0 || ny < 0 || nx >= a || ny >= b) continue;
                if(dst[nx][ny] != inf) continue;
                //add blocking conditions, if need
                
                dst[nx][ny] = dst[x][y] + 1;
            }
        }
    }

    vec<vec<int>> go(vec<vec<int>> &m, const pii st, const int inf = numeric_limits<int>::max()) {
        vec<vec<int>> dst(m.size(), vec<int>(m[0].size()));
        go(m, st, dst, inf);
        return dst;
    }
};