/*
Karger-Stein algorithm for finding global min cut in undirected weighted graph
Idea:
  Choose random edge, contract it. Repeat until V / D vertexes left. Now do the same process recursively 2 times.
  When 2 vertexes left, chmin current min_cut with the weight of edges between them.
  Probability that we found the optimal answer is >= 1 / log(V) with D = sqrt(2).
Complexity: O(V^2log(V)) for 1 iteration of algorithm with D = sqrt(2). We need O(log(V)log(eps)) iterations to guarantee P(wrong answer) < 1 / eps. 
Problems:
https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=21&page=show_problem&problem=1930
*/
template<typename T>
class karger_stein {
    struct edge {
        int x, y, sx, sy;
        T weight;
        edge(int x, int y, T weight): x(x), y(y), sx(x), sy(y), weight(weight) {}
    };

    size_t V;
    vector<edge> store;
    vector<vector<int>> l;

public:
    karger_stein(size_t V): V(V), l(V) {
        assert(V >= 2);
    }

    void clear() {
        store.clear();
        for (auto& i : l) i.clear(); 
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
    pair<T, vector<array<T, 3>>> calc_global_min_cut(double D = sqrt(2), int ITER = -1) {
        if (ITER == -1) ITER = (__lg(V) + 1) * 10;
        assert(D > 1 && ITER > 0);
        vector<int> p(store.size()), pr(V); iota(p.begin(), p.end(), 0); iota(pr.begin(), pr.end(), 0);
        function<int(int)> find = [&](int v) {return pr[v] == v ? v : find(pr[v]);};
        mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<int> gen(0, 1 << 30);
        T min_cut = numeric_limits<T>::max();
        vector<array<T, 3>> ans;
        function<void(int, int)> go = [&](int sz, int pos) {
            if (sz == 2) {
                T tyt = 0;
                int v = find(0);
                for (int i : l[v]) {
                    int to = store[i].x + store[i].y - v;
                    if (to == v) continue;
                    tyt += store[i].weight;
                }
                if (tyt < min_cut) {
                    min_cut = tyt;
                    ans.clear();
                    for (int i : l[v]) {
                        int to = store[i].x + store[i].y - v;
                        if (to == v) continue;
                        ans.push_back({store[i].sx, store[i].sy, store[i].weight});
                    }
                }
                return;
            }
            if (pos == p.size()) {min_cut = 0; return;}
            int stop = max<int>(2, sz / D);
            vector<array<int, 3>> cng;
            vector<pair<int, int>> cx, cy;
            for (; sz > stop && pos < p.size();) {
                swap(p[pos], p[pos + gen(rnd) % (p.size() - pos)]);  //You might wanna select not just random edge, but with correspondence to their weights.
                auto e = store[p[pos++]];
                if (e.x == e.y) continue;
                if (l[e.x].size() > l[e.y].size()) swap(e.x, e.y);
                --sz;
                pr[e.x] = e.y;
                int s = 0;
                for (int i : l[e.x]) {
                    if (store[i].x == e.x) store[i].x = e.y, cx.emplace_back(i, e.x);
                    if (store[i].y == e.x) store[i].y = e.y, cy.emplace_back(i, e.x);
                    if (store[i].x != store[i].y) l[e.y].push_back(i), ++s;
                }
                cng.push_back({e.x, e.y, s});
            }
            go(sz, pos);
            go(sz, pos);
            while (cng.size()) {
                auto [x, y, s] = cng.back(); cng.pop_back();
                pr[x] = x;
                l[y].erase(l[y].end() - s, l[y].end());
            }
            for (int i = int(cx.size()) - 1; i >= 0; --i) store[cx[i].first].x = cx[i].second;
            for (int i = int(cy.size()) - 1; i >= 0; --i) store[cy[i].first].y = cy[i].second;
            cx.clear(), cy.clear();
        };
        for (int it = 0; it < ITER; ++it) go(V, 0);
        return {min_cut, ans};
    }
};
