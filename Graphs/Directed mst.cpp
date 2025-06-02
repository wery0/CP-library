template<class T>
class directed_mst {
    struct dsu {
        vector<int> p;
        dsu(int n): p(n, -1) {}
        int find(int i) {return p[i] < 0 ? i : p[i] = find(p[i]);}
        bool same(int i, int j) {return find(i) == find(j);}
        int size(int i) { return -p[find(i)];}
        bool join(int i, int j) {
            i = find(i), j = find(j);
            if (i == j) return false;
            if (p[i] > p[j]) swap(i, j);
            p[i] += p[j], p[j] = i;
            return true;
        }
    };

    struct rollback_dsu {
        vector<int> p;
        vector<pair<int, int>> joins;

        rollback_dsu(int n): p(n, -1) {}
        int find(int i) const {return p[i] < 0 ? i : find(p[i]);}
        bool same(int i, int j) const {return find(i) == find(j);}
        int size(int i) const {return -p[find(i)];}
        int time() const {return joins.size();}

        bool join(int i, int j) {
            i = find(i), j = find(j);
            if (i == j) return false;
            if (p[i] > p[j]) swap(i, j);
            joins.emplace_back(j, p[j]);
            p[i] += p[j], p[j] = i;
            return true;
        }

        void rollback(int t) {
            while (time() > t) {
                auto [i, pi] = joins.back(); joins.pop_back();
                assert(p[p[i]] < 0);
                p[p[i]] -= pi;
                p[i] = pi;
            }
        }
    };

    struct Node {int l, r, from, to; T weight, lz;};
    size_t n;
    vector<Node> nodes;
    vector<int> heap;

    void apply(int i, T upd) {nodes[i].weight -= upd; nodes[i].lz += upd;}
    void push(int i) {
        if (nodes[i].l != -1) apply(nodes[i].l, nodes[i].lz);
        if (nodes[i].r != -1) apply(nodes[i].r, nodes[i].lz);
        nodes[i].lz = 0;
    }
    int merge(int u, int v) {
        if (u == -1 || v == -1) return (u == -1 ? v : u);
        if (nodes[v].weight < nodes[u].weight) swap(u, v);
        push(u);
        nodes[u].r = merge(nodes[u].r, v);
        swap(nodes[u].l, nodes[u].r);
        return u;
    }
    void pop(int v) {
        push(heap[v]);
        heap[v] = merge(nodes[heap[v]].l, nodes[heap[v]].r);
    }

public:
    directed_mst(size_t n, size_t k = 0): n(n), heap(n, -1) {nodes.reserve(k);}

    void add_edge(size_t from, size_t to, T weight) {
        assert(from < n && to < n);
        nodes.push_back({ -1, -1, int(from), int(to), weight, T(0)});
        heap[to] = merge(heap[to], nodes.size() - 1);
    }

    //Returns {min ordered mst weight, vector of parents for nodes in mst}
    //If not all vertexes are reachable from root, returns {0, {}}
    template<typename C = T>
    pair<C, vector<int>> solve(size_t root) {
        assert(root < n);
        C ans = 0;
        vector edge(heap.size(), -1);
        vector<pair<int, int>> cycles;
        dsu dsu_cyc(heap.size());
        rollback_dsu dsu_contract(heap.size());
        for (size_t i = 0; i < n; ++i) {
            if (i == root) continue;
            int v = i;
            while (true) {
                if (heap[v] == -1) return {T(0), {}};
                edge[v] = heap[v];
                ans += nodes[edge[v]].weight;
                apply(edge[v], nodes[edge[v]].weight);
                if (dsu_cyc.join(v, dsu_contract.find(nodes[edge[v]].from))) {
                    break;
                }

                int vnext = dsu_contract.find(nodes[edge[v]].from), t = dsu_contract.time();
                while (dsu_contract.join(v, vnext)) {
                    heap[dsu_contract.find(v)] = merge(heap[v], heap[vnext]);
                    v = dsu_contract.find(v);
                    vnext = dsu_contract.find(nodes[edge[vnext]].from);
                }
                cycles.emplace_back(edge[v], t);

                while (heap[v] != -1 && dsu_contract.same(nodes[heap[v]].from, v)) {
                    pop(v);
                }
            }
        }

        for (auto it = rbegin(cycles); it != rend(cycles); ++it) {
            int vrepr = dsu_contract.find(nodes[it->first].to);
            dsu_contract.rollback(it->second);
            int vinc = dsu_contract.find(nodes[edge[vrepr]].to);
            edge[vinc] = exchange(edge[vrepr], it->first);
        }

        for (size_t i = 0; i < n; ++i) {
            edge[i] = i == root ? -1 : nodes[edge[i]].from;
        }
        return {ans, edge};
    }
};
