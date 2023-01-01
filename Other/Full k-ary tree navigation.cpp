//If root of tree is 1
ll leftest_child_of_node_in_full_k_ary_tree(ll k, ll n) {return k * (n - 1) + 2;}
ll rightest_child_of_node_in_full_k_ary_tree(ll k, ll n) {return k * n + 1;}
ll parent_of_node_in_full_k_ary_tree(ll k, ll n) {return (n + k - 2) / k;}