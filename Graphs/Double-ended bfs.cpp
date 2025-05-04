struct action {
    char c = 0;

    action() = default;
    action(char c): c(c) {}
    
    void inverse() {
        //Implement
        assert(0);
    }
    bool is_doable_from(int from) {
        assert(from == 1 || from == 2);
        //Implement
        return ?;
    }
    
    bool operator==(const action& rhs) const {return c == rhs.c;}
    action get_inverse() const {action res = *this; res.inverse(); return res;}
    string to_string() const {return string(1, c);}
    friend ostream& operator<<(ostream& out, const action& rhs) {return out << rhs.to_string();}
};

vector<action> possible_actions;

struct state {

    struct hsh {
        size_t operator()(const state& s) const {
            //Implement
            return ?;
        }
    };

    state() = default;
    //Implement ctors

    bool operator==(const state& rhs) const {
        //Implement
        return ?;
    }
    bool operator<(const state& rhs) const {
        //Implement
        return ?;
    }

    bool can_apply_action(const action& a) const {
        //Implement
        return ?;
    }

    void apply_action(const action& a) {
        assert(can_apply_action(a));
        //Implement
        assert(0);
    }

    int calc_distance_from(const state& rhs) const {
        int res = 0;
        //Implement if need
        return res;
    }

    string to_string() const {
        string res;
        //Implement if need
        return res;
    }

    void undo_action(const action& a) {
        apply_action(a.get_inverse());
    }

    vector<pair<action, state>> get_adjacent_states(int from) const {
        vector<pair<action, state>> res;
        auto process = [&](action a) {
            if (a.is_doable_from(from) && can_apply_action(a)) {
                state ns = *this;
                ns.apply_action(a);
                res.emplace_back(a, ns);
            }
        };
        for (action a : possible_actions) {
            process(a);
        }
        return res;
    }

    friend ostream& operator<<(ostream& out, const state& rhs) {return out << rhs.to_string();}
};

/*
  If A is closer to 0, then path is searching slower, but it is shorter. A = 0 will find the shortest possible path.
  If A is closer to 1, then path is searching faster, but it is longer.
*/
const double A = 0.7;
uniform_real_distribution<double> gen(0, 1);

int main() {
    {
        //Fill possible_actions
    }
    //Fill start_states and end_states
    vector<state> start_states;
    vector<state> end_states;
    assert(start_states.size() && end_states.size());
    cout << "START STATES:\n"; for (auto s : start_states) cout << s << '\n'; cout << '\n';
    cout << "END STATES:\n"; for (auto s : end_states) cout << s << '\n'; cout << endl;

    unordered_map<state, pair<uint8_t, action>, state::hsh> us(1 << 25);  //Adjust initial cache size if needed
    deque<state> dq(start_states.begin(), start_states.end());
    dq.insert(dq.end(), end_states.begin(), end_states.end());
    bool found = false;
    state ans_start_state, ans_end_state;
    vector<action> ans_actions;
    for (auto s : start_states) us[s].first = 1;
    for (auto s : end_states) {
        if (us[s].first == 1) found = true, ans_start_state = s, ans_end_state = s, us[s].first = 3;
        else us[s].first = 2;
    }
    uint64_t visited_states = 0, looked_again_states = 0;
    const uint64_t STEP = 5e6;
    auto start_time = timeStamp();
    for (uint64_t d = 0, eso = dq.size(), thr = 0; dq.size() && !found;) {
        if (eso == 0) eso = dq.size(), ++d; --eso;
        if (us.size() > thr) {cout << "CACHE SIZE: " << thr / 1e6 << "M, time " << duration_milli(timeStamp() - start_time) / 1000.0 << "s" << endl; thr += STEP;}
        state cur_state = dq.front(); dq.pop_front();
        auto [from, last_action] = us[cur_state];
        auto states_to_explore = cur_state.get_adjacent_states(from);
        if (d > 1 && gen(rnd) < A) {
            auto cmp = [&](const auto& l, const auto& r) {
                const auto& m = from == 1 ? end_states : start_states;
                auto dl = l.second.calc_distance_from(m[0]);
                auto dr = r.second.calc_distance_from(m[0]);
                return dl < dr;
            };
            int cnt = min<int>(states_to_explore.size(), gen(rnd) < 0.5 ? 1 : 2);
            nth_element(states_to_explore.begin(), states_to_explore.begin() + cnt, states_to_explore.end(), cmp);
            states_to_explore.resize(cnt);
        }
        for (auto [action, next_state] : states_to_explore) {
            auto& [nm, npa] = us[next_state];
            if (nm != from) {
                if (nm == 0) {
                    nm = from, npa = action;
                    dq.emplace_back(next_state);
                    visited_states += 1;
                    // cout << cur_state << " -(" << int(from) << ' ' << action << ")> " << next_state << endl;
                } else {
                    assert(nm == 3 - from);
                    cout << "FOUND!" << endl;
                    found = true;
                    ans_start_state = from == 1 ? cur_state : next_state;
                    while (find(start_states.begin(), start_states.end(), ans_start_state) == start_states.end()) {
                        assert(us.count(ans_start_state) && "Check your undoing!");
                        ans_actions.push_back(us[ans_start_state].second);
                        ans_start_state.undo_action(us[ans_start_state].second);
                    }
                    reverse(ans_actions.begin(), ans_actions.end());
                    ans_actions.push_back(from == 1 ? action : action.get_inverse());
                    ans_end_state = from == 1 ? next_state : cur_state;
                    while (find(end_states.begin(), end_states.end(), ans_end_state) == end_states.end()) {
                        assert(us.count(ans_end_state) && "Check your undoing!");
                        ans_actions.push_back(us[ans_end_state].second.get_inverse());
                        ans_end_state.undo_action(us[ans_end_state].second);
                    }
                    break;
                }
            } else {
                ++looked_again_states;
            }
        }
    }
    cout << "VISITED STATES: " << visited_states << '\n';
    cout << "CACHE SIZE: " << us.size() << '\n';
    cout << "LOOKED AGAIN STATES: " << looked_again_states << '\n';
    double t = duration_milli(timeStamp() - start_time) / 1000.0;
    cout << "TIME: " << t << "s" << '\n';
    cout << "SPEED: " << int((visited_states + looked_again_states) / t) / 10000 * 10000 / 1e6 << "M states / second\n\n";
    if (found) {
        cout << "FOUND!\n";
        cout << "START STATE: " << ans_start_state << '\n';
        cout << "END STATE: " << ans_end_state << '\n';
        cout << "ACTIONS (" << ans_actions.size() << "): "; for (action a : ans_actions) cout << a; cout << '\n';
    } else {
        cout << "NOT FOUND!";
    }
    cout << endl;
}
