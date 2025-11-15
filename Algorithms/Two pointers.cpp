auto two_pointers_bad_to_good = [&]() {
    const int N = ?;
    vector<int> first_good(N);
    int l = 0, r = -1;
    auto is_good = [&]() -> bool {
        if (l > r) return false;
        if (r == N) return true;
        //vvv write vvv

    };
    auto inc_l = [&]() -> void {

        //^^^ remove l ^^^
        ++l;
    };
    auto inc_r = [&]() -> void {
        if (++r == N) return;
        //vvv add r vvv

    };
    for (; l < N; inc_l()) {
        while (!is_good()) inc_r();
        first_good[l] = r;
    }
    return first_good;  //for each i, segments [i, r] (first_good[i] <= r <= N - 1) are good
};

auto two_pointers_good_to_bad = [&]() {
    const int N = ?;
    vector<int> good_until(N);
    int l = 0, r = -1;
    auto is_good = [&]() -> bool {
        if (l > r) return true;
        if (r == N) return false;
        //vvv write vvv

    };
    auto inc_l = [&]() -> void {

        //^^^ remove l ^^^
        ++l;
    };
    auto inc_r = [&]() -> void {
        if (++r == N) return;
        //vvv add r vvv

    };
    for (; l < N; inc_l()) {
        while (is_good()) inc_r();
        good_until[l] = r - 1;
    }
    return good_until;  //for each i, segments [i, r] (i <= r <= good_until[i]) are good
};
