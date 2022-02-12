struct XORShift {
    ll seed;
    XORShift(int seed) {this->seed = seed; }
    XORShift() { this->seed = std::chrono::steady_clock::now().time_since_epoch().count(); }
    ll genRandomNumber() {
        seed ^= seed << 21;
        seed ^= seed >> 35u;
        seed ^= seed << 4;
        return seed;
    }
    ll genRandomPositiveNumber(ll maximal) {
        return (abs(genRandomNumber()) + maximal) % maximal;
    }
};
