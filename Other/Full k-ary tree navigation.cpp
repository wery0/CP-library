//If root of tree is 1
template<typename T> T leftest_child_of_node_in_full_k_ary_tree(T k, T n) {return k * (n - 1) + 2;}
template<typename T> T rightest_child_of_node_in_full_k_ary_tree(T k, T n) {return k * n + 1;}
template<typename T> T parent_of_node_in_full_k_ary_tree(T k, T n) {return (n + k - 2) / k;}