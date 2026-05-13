//{f(0), f(1), f(2), f(3), f(4), f(5), f(6), f(7)} = {0b000, 0b001, 0b011, 0b010, 0b110, 0b111, 0b101, 0b100}
template<typename T> T num_to_gray_code(T n) {return n ^ (n >> 1);}

template<typename T> T gray_code_to_num(T g) {T n = 0; for (; g; g >>= 1) n ^= g; return n;}
