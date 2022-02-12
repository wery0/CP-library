#include<ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;

const int RND = rnd();
struct hsh{
    int operator()(int x) const{return x^RND;}
};

gp_hash_table<int, int, hsh> G;




//WARNING: if X doesn`t exist in G, X will be added after query G[X].
//Soln to this: G.find(x)!=G.end()