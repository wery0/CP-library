auto simulated_annealing = [&](bool maximize_score = true) {
    auto calc_score = [](const auto & state) {

    };
    maximize_score ^= 1;
    uniform_real_distribution<double> gen(0, 1);
    mt19937 rnd(777);
    const int sign = maximize_score ? -1 : 1;

    const double TL = 1.0;
    const int ATTEMPTS = 20;
    const double TL_RUN = TL / ATTEMPTS / 2;
    const auto WIN_SCORE = ?;
    for (int _ = 0; _ < ATTEMPTS; ++_) {
        //Build cur_state
        auto cur_state;

        auto best_state = cur_state;
        auto cur_score = calc_score(cur_state);
        auto best_score = cur_score;

        const double t_start = 10;
        const double t_end = 0.001;
        for (auto st = timeStamp(); cur_score != WIN_SCORE && (cur_score < WIN_SCORE) ^ maximize_score;) {
            double elapsed_frac = duration_micro(timeStamp() - st) / (TL_RUN * 1e6);
            if (elapsed_frac > 1) break;
            double T = t_start * pow(t_end / t_start, elapsed_frac);
            //Change cur_state

            auto new_score = calc_score(cur_state);
            if (new_score == cur_score || (new_score >= cur_score) ^ maximize_score ||
                    gen(rnd) < exp(sign * (new_score - cur_score) / T)) {
                cur_score = new_score;
                if (cur_score != best_score && (cur_score > best_score) ^ maximize_score) {
                    best_score = cur_score;
                    best_state = cur_state;
                }
            } else {
                //Revert cur_state back
            }
        }
        if (cur_score == WIN_SCORE || (best_score >= WIN_SCORE) ^ maximize_score) {
            return best_state;
        }
    }
    return ?;
};
simulated_annealing(true);