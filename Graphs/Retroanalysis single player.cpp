/*
For each vertex v of a directed graph calculates {status, steps} which means:
  - status = 1  => you can reach winning vertex and shortest path will take steps moves
  - status = 0  => you can't reach any winning vertex but you can avoid all losing ones forever, steps = 0
  - status = -1 => no matter where you go, you will arrive to a losing vertex and longest path will take steps moves
Complexity: O(V + E)
Problems:
  https://codeforces.com/contest/48/problem/E
*/
class retroanalysis_single_player {
    size_t V;
    vector<vector<int>> il;
    vector<array<int, 2>> state;
    bool was_analyzed = false;

public:
    retroanalysis_single_player() = default;
    retroanalysis_single_player(size_t V): V(V), il(V), state(V, {0, 0}) {}

    void add_edge(size_t from, size_t to) {
        assert(!was_analyzed);
        assert(from < V && to < V);
        il[to].push_back(from);
    }

    void mark_terminal_vertex(size_t v, int type) {
        assert(!was_analyzed);
        assert(type == -1 || type == 1);
        assert(state[v][0] == 0 && "Vertex is marked twice!");
        state[v][0] = type;
    }

    vector<array<int, 2>> analyze() {
        assert(!was_analyzed);
        vector<int> deg(V);
        for (int v = 0; v < V; ++v) for (int from : il[v]) ++deg[from];
        deque<int> dq;
        for (int v = 0; v < V; ++v) if (state[v][0] == 1) dq.push_back(v);
        for (; dq.size(); ) {
            int v = dq.front(); dq.pop_front();
            for (int h : il[v]) {
                if (state[h][0] == -1) {
                    assert(0 && "Winning state is reachable from losing one!");
                } else if (state[h][0] == 0) {
                    state[h] = {1, state[v][1] + 1};
                    dq.push_back(h);
                } else if (state[h][0] == 1) {
                    continue;
                } else assert(0);
            }
        }
        for (int v = 0; v < V; ++v) if (state[v][0] == -1) dq.push_back(v);
        for (; dq.size(); ) {
            int v = dq.front(); dq.pop_front();
            for (int h : il[v]) {
                if (state[h][0] == -1) {
                    continue;
                } else if (state[h][0] == 0) {
                    state[h][1] = max(state[h][1], state[v][1] + 1);
                    if (--deg[h] == 0) {
                        state[h][0] = -1;
                        dq.push_back(h);
                    }
                } else if (state[h][0] == 1) {
                    continue;
                } else assert(0);
            }
        }
        for (int v = 0; v < V; ++v) if (state[v][0] == 0) state[v][1] = 0;
        was_analyzed = true;
        return state;
    }
};
