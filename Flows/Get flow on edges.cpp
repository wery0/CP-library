//For every added edge returns the flow going through it.
//If the flow on edge x->y is negative, then the flow goes from y to x (could only happen with undirected edges)
vector<tuple<int, int, T_flow>> get_flow_on_edges() const {
    vector<tuple<int, int, T_flow>> res(store.size() / 2);
    for (size_t i = 0; i < store.size(); i += 2) {
        const auto& e = store[i];
        res[i / 2] = {store[i ^ 1].to, e.to, e.flow};
    }
    return res;
}
