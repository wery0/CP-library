/*
Stoer-Wagner algorithm for finding global min cut in undirected weighted graph
Idea:
  Mark any vertex. Then mark unmarked vertex which is the most connected to the set of marked vertex. 
  Let vertex S be the last marked and T second to last marked. Calculate (S, T) max flow (== min cut), update the answer, merge S and T. Repeat until only 1 vertex left.
  Crucial idea is that (S, T) max flow == sum of adjacent edges to S so there is no need to use any flow finding algorithms.
Complexity: O(VElog(V))
Problems:
https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=21&page=show_problem&problem=1930
*/
template<typename T>
class stoer_wagner {
    struct edge {
        int x, y, sx, sy;
        T weight;
        edge(int x, int y, T weight): x(x), y(y), sx(x), sy(y), weight(weight) {}
    };

    size_t V;
    vector<edge> store;
    vector<vector<int>> l;

public:
    stoer_wagner(size_t V): V(V), l(V) {
        assert(V >= 2);
    }

    //Edge is undirected
    void add_edge(int x, int y, T weight) {
        assert(0 <= min(x, y) && max(x, y) < V);
        if (x == y) return;
        l[x].push_back(store.size());
        l[y].push_back(store.size());
        store.emplace_back(x, y, weight);
    }

    //Returns {weight of min cut, vector of edges in min cut in form {x, y, weight}}
    pair<T, vector<array<T, 3>>> calc_global_min_cut() {
        T min_cut = numeric_limits<T>::max();
        vector<int> us(V, -1), pr(V); iota(pr.begin(), pr.end(), 0);
        function<int(int)> find = [&](int v) {return pr[v] = pr[v] == v ? v : find(pr[v]);};
        vector<T> sm(V);
        priority_queue<pair<T, int>, vector<pair<T, int>>> pq;
        vector<array<T, 3>> ans;
        for (int it = 0, s, t; it < V - 1; ++it) {
            fill(sm.begin(), sm.end(), 0);
            for (int v = 0; v < V; ++v) if (pr[v] == v) pq.emplace(0, v);
            while (pq.size()) {
                auto [sw, v] = pq.top(); pq.pop();
                if (us[v] == it) continue;
                us[v] = it;
                for (int i : l[v]) {
                    auto& e = store[i];
                    int to = e.x + e.y - v;
                    if (us[to] == it) continue;
                    sm[to] += e.weight;
                    pq.emplace(sm[to], to);
                }
                t = s, s = v;
            }
            T st_max_flow = 0;
            for (int i : l[s]) {
                int to = store[i].x + store[i].y - s;
                if (to != s) st_max_flow += store[i].weight;
            }
            if (st_max_flow < min_cut) {
                min_cut = st_max_flow;
                ans.clear();
                for (int i : l[s]) {
                    if (store[i].x != store[i].y) ans.push_back({store[i].sx, store[i].sy, store[i].weight});
                }
            }
            if (l[s].size() < l[t].size()) swap(s, t);
            pr[t] = s;
            for (int i : l[t]) {
                auto& e = store[i];
                if (e.x = find(e.x), e.y = find(e.y); e.x != e.y) l[s].push_back(i);
            }
            l[t].clear(); l[t].shrink_to_fit();
        }
        return {min_cut, ans};
    }
};
