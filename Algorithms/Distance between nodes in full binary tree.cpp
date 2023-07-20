//Root of tree has number 1.
//O(1)
template<typename T>
T distance_between_nodes_in_full_binary_tree(T x, T y) {
    if (x > y) swap(x, y);
    T ans = __lg(y) - __lg(x);
    y >>= ans;
    return ans + (x == y ? 0 : __lg(x ^ y) + 1) * 2;
}