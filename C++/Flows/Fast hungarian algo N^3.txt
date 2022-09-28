template <typename Cost = long, typename CostSum = Cost>
auto fast_dense_hungarian(const vector<vector<Cost>>& cost) {
    static constexpr CostSum inf = numeric_limits<CostSum>::max() / 3;
    int V = cost.size();

    vector<int> row_mate(V, -1), col_mate(V, -1);
    vector<CostSum> pi(V, 0);

    auto residual = [&](int r, int c) { return cost[r][c] - pi[c]; };

    // column reduction, mate columns greedily
    vector<bool> transferrable(V, false);
    for (int col = 0; col < V; col++) {
        int row = 0;
        for (int u = 1; u < V; u++) {
            if (cost[row][col] > cost[u][col]) {
                row = u;
            }
        }
        pi[col] = cost[row][col];
        if (row_mate[row] == -1) {
            row_mate[row] = col;
            col_mate[col] = row;
            transferrable[row] = true;
        } else {
            transferrable[row] = false;
        }
    }

    // reduction transfer
    for (int row = 0; row < V; row++) {
        if (transferrable[row]) {
            int col = row_mate[row];
            int c = -1;
            for (int v = 0; v < V; v++) {
                if (v != col && (c == -1 || residual(row, c) > residual(row, v))) {
                    c = v;
                }
            }
            pi[col] -= residual(row, c);
        }
    }

    // augmenting row reduction
    for (int it = 0; it < 2; it++) {
        for (int row = 0; row < V; row++) {
            if (row_mate[row] != -1) {
                continue;
            }
            auto u1 = residual(row, 0);
            auto u2 = inf;
            int c1 = 0;
            for (int c = 0; c < V; c++) {
                auto u = residual(row, c);
                if (u < u1 || (u == u1 && col_mate[c1] != -1)) {
                    u2 = u1, u1 = u, c1 = c;
                } else if (u < u2) {
                    u2 = u;
                }
            }
            if (u1 < u2) {
                pi[c1] -= u2 - u1;
            }
            if (int r1 = col_mate[c1]; r1 != -1) {
                row_mate[r1] = col_mate[c1] = -1;
            }
            row_mate[row] = c1;
            col_mate[c1] = row;
        }
    }

    vector<int> cols(V);
    iota(begin(cols), end(cols), 0);

    for (int row = 0; row < V; row++) {
        if (row_mate[row] != -1) {
            continue;
        }
        vector<CostSum> dist(V);
        for (int c = 0; c < V; c++) {
            dist[c] = residual(row, c);
        }
        vector<int> pred(V, row);

        int scanned = 0, labeled = 0, last = 0;
        int col = -1;

        while (true) {
            if (scanned == labeled) {
                last = scanned;
                auto min = dist[cols[scanned]];
                for (int j = scanned; j < V; j++) {
                    int c = cols[j];
                    if (dist[c] <= min) {
                        if (dist[c] < min) {
                            min = dist[c];
                            labeled = scanned;
                        }
                        swap(cols[j], cols[labeled++]);
                    }
                }
                for (int j = scanned; j < labeled; j++) {
                    if (int c = cols[j]; col_mate[c] == -1) {
                        col = c;
                        goto done;
                    }
                }
            }
            assert(scanned < labeled);
            int c1 = cols[scanned++];
            int r1 = col_mate[c1];
            for (int j = labeled; j < V; j++) {
                int c2 = cols[j];
                auto len = residual(r1, c2) - residual(r1, c1);
                assert(len >= 0);
                if (dist[c2] > dist[c1] + len) {
                    dist[c2] = dist[c1] + len;
                    pred[c2] = r1;
                    if (len == 0) {
                        if (col_mate[c2] == -1) {
                            col = c2;
                            goto done;
                        }
                        swap(cols[j], cols[labeled++]);
                    }
                }
            }
        }

done:;
        for (int i = 0; i < last; i++) {
            int c = cols[i];
            pi[c] += dist[c] - dist[col];
        }

        int t = col;
        while (t != -1) {
            col = t;
            int r = pred[col];
            col_mate[col] = r;
            swap(row_mate[r], t);
        }
    }

    CostSum total = 0;
    for (int u = 0; u < V; u++) {
        total += cost[u][row_mate[u]];
    }
    return make_pair(total, move(row_mate));
}