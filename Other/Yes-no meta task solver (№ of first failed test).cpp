//This program solves a following meta-task: given a YES/NO problem and judge gives you the № of first failed testcase. Solve the problem.
/*
How to use:
1). Change YES and NO variables to corresponding values if needed.
2). Write a code inside `head` variable that reads input and puts it into `data` variable.
3). Run the program.
while not accepted
    1). Send the program from output.
    2). Enter the № of first test that failed or -1 if all tests have passed.
*/
#include "bits/stdc++.h"
using namespace std;

const string YES = "1";
const string NO = "0";

template<typename T>
string calc_hash(const vector<T>& data) {
    T p = 2807516534892679321, mod = 3835424442118071511, hs = 0;
    for (T pw = 1; T c : data) {
        hs = (hs + c * pw) % mod;
        pw = pw * p % mod;
    }
    string o;
    for (int i = 0; i < 64; ++i) o += '0' + (hs >> i & 1);
    return o;
}

string head = R"(#include "bits/stdc++.h"
using namespace std;

template<typename T>
string calc_hash(const vector<T>& data) {
    T p = 2807516534892679321, mod = 3835424442118071511, hs = 0;
    for (T pw = 1; T c : data) {
        hs = (hs + c * pw) % mod;
        pw = pw * p % mod;
    }
    string o;
    for (int i = 0; i < 64; ++i) o += '0' + (hs >> i & 1);
    return o;
}

const string YES = ")" + YES + R"(";
const string NO = ")" + NO + R"(";

int main() {
    using T = uint64_t;
    vector<T> data;
    {
        //Read data
    }
    string hs = calc_hash<T>(data);
)";

struct command {
    int type = -1;
    vector<string> m;

    string as_string() {
        if (type == 0) return "cout << " + (m[0] == "0" ? NO : YES) + " << endl;";
        if (type == 1) return "if (hs.substr(0, " + to_string(m[0].size()) + ") == \"" + m[0] + "\") {cout << " + (m[1] == "0" ? NO : YES) + " << endl; return 0;}";
        if (type == 2) return "cout << (hs[" + m[0] + "] == \'0\' ? NO : YES) << endl;";
        assert(0);
        return "";
    }
};

string make_program(vector<command> commands, bool do_compress) {
    string res = head;
    for (auto command : commands) {
        res += "    " + command.as_string() + "\n";
    }
    res += "}\n";
    if (do_compress) {
        string o;
        for (int cnt = 0; char c : res) {
            if (c == '\n') {
                if (++cnt == 1) o += c;
            } else if (o.empty() || o.back() != ' ' || c != ' ') o += c;
        }
        res = o;
    }
    return res;
}

int solve(auto _eval) {
    int cnt_evals = 0;
    auto eval = [&](const vector<command>& commands) {
        ++cnt_evals;
        return _eval(commands);
    };
    auto concat = [](vector<command> l, vector<command> r) {
        l.insert(l.end(), r.begin(), r.end());
        return l;
    };
    vector<int> test_answers;
    vector<string> bits;
    vector<command> commands;
    bool done = false;
    for (int t = 1; !done; ++t) {
        int r0 = eval(concat(commands, {{0, {"0"}}}));
        if (done |= r0 == -1; done) break;
        int r1 = eval(concat(commands, {{0, {"1"}}}));
        if (done |= r1 == -1; done) break;
        if (r0 == r1 || min(r0, r1) > t) {
            assert(t > 1 && "Smth wrong. Check YES and NO variables.");
            assert(bits.size() == commands.size());
            for (int i = 0; i < bits.size(); ++i) {
                auto nc = commands;
                nc.resize(i);
                nc.push_back({2, {to_string(bits[i].size())}});
                int e = eval(nc);
                if (done |= e == -1; done) break;
                bits[i] += '0' + (test_answers[i] ^ (e == i + 1));
                commands[i].m[0] = bits[i];
            }
            --t;
            continue;
        }
        test_answers.push_back(r0 == t ? 1 : 0);
        bits.push_back(string(bits.empty() ? 0 : bits.back().size(), '?'));
        string hs;
        for (int i = 0; i < bits.back().size(); ++i) {
            int e = eval(concat(commands, {{2, {to_string(i)}}}));
            if (done |= e == -1; done) break;
            assert(e >= t);
            bits.back()[i] = '0' + (test_answers.back() ^ (e == t));
        }
        commands.push_back({1, {bits.back(), to_string(test_answers.back())}});
    }
    return cnt_evals;
}

void calc_expected_number_of_submissions(int tests, int samples) {
    int seed;
    map<int, vector<string>> mp;
    uniform_int_distribution<uint64_t> g(0, 1ll << 30);
    auto get_hashes_by_seed = [&](int seed) {
        if (mp.count(seed)) return mp[seed];
        vector<vector<uint64_t>> datas(tests);
        vector<string> hashes;
        mt19937 rnd(seed);
        for (int i = 0; i < datas.size(); ++i) {
            datas[i].push_back(g(rnd));
            hashes.push_back(calc_hash(datas[i]));
        }
        return mp[seed] = hashes;
    };
    auto test_evaluator = [&](const vector<command>& commands) {
        auto hashes = get_hashes_by_seed(seed);
        auto real_answer = [&](int tc) {
            return tc % 2 == 0;
        };
        int o = -1;
        for (int i = 0; i < hashes.size(); ++i) {
            string ans;
            for (auto command: commands) {
                if (command.type == 0) {
                    ans += command.m[0];
                } else if (command.type == 1) {
                    if (hashes[i].substr(0, command.m[0].size()) == command.m[0]) {
                        ans = command.m[1];
                        break;
                    }
                } else if (command.type == 2){
                    ans += hashes[i][stoi(command.m[0])] == '0' ? "0" : "1";
                } else assert(0);
            }
            if (ans != to_string(real_answer(i + 1))) {
                o = i + 1;
                break;
            }
        }
        return o;
    };
    vector<int> m(samples);
    for (int i = 0; i < samples; ++i) {
        seed = i;
        m[i] = solve(test_evaluator);
    }
    sort(m.begin(), m.end());
    vector<double> thr = {0.0001, 0.001, 0.01, 0.05, 0.1, 0.25, 0.5, 0.75, 0.9, 0.95, 0.99, 0.999};
    for (int l = 0, i = 0; l < samples; ++l) {
        int r = l;
        while (r + 1 < samples && m[r + 1] == m[l]) ++r;
        double prob = double(r + 1) / samples;
        if (i < thr.size() && prob >= thr[i]) {
            ++i;
            cout << m[r] << " submissions give you " << prob * 100 << "% chance to get AC for " << tests << " tests\n";
        }
        l = r;
    }
}

int main() {
    //calc_expected_number_of_submissions(?, 1000); return 0;          //Uncomment this line to estimate needed number of submissions to get AC. Ensure that calc_hash() function is the same as in `head` variable. Change number of samples in case it works too long.
    auto real_evaluator = [](const vector<command>& commands) {
        string program = make_program(commands, true);
        cout << program << endl;
        int x; cin >> x;
        return x;
    };
    solve(real_evaluator);
}
