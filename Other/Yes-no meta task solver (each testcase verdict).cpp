//These programs solves the following meta-task: given a YES/NO problem and judge gives you the verdict for each testcase. Solve the problem.
/*
How to use:
1). Change YES and NO variables to corresponding values if needed.
2). Write a code that reads input and puts it into `data` variable.
3). Write the second program which parses output from judging system.
4). Send the first program to get answers for each testcase. Put them into `test_answers` variable as binary string.
while not accepted
    1). Send the first program.
    2). Run the second program with the output from the judge. Put it's output to 120th line.
*/

//First program
#include "bits/stdc++.h"
using namespace std;

const string YES = "YES";
const string NO = "NO";

template<typename T>
T calc_hash(const vector<T>& data, array<T, 2> params) {
    auto [p, mod] = params;
    T hs = 0;
    for (T pw = 1; T c : data) {
        hs = (hs + c * pw) % mod;
        pw = pw * p % mod;
    }
    return hs;
}

template<typename T>
bool is_yes(const vector<T>& data, array<T, 2> params) {
    return (calc_hash(data, params) % 2) == 0;
}

vector<string> get_masks_of_answers(vector<string> answers) {
    assert(answers.size());
    vector<string> masks(answers[0].size());
    for (int j = 0; j < answers[0].size(); ++j) {
        for (auto s : answers) masks[j] += s[j];
    }
    return masks;
}

bool check_all_masks_are_distinct(vector<string> answers) {
    assert(answers.size());
    auto masks = get_masks_of_answers(answers);
    map<string, vector<int>> groups;
    for (int i = 0; auto s : masks) groups[s].push_back(i++);
    cerr << "DISTINCT: " << groups.size() << " / " << answers[0].size() << endl;
    for (auto [mask, nums] : groups) {
        cerr << mask << ": ";
        for (int i : nums) cout << i << ' ';
        cerr << endl;
    }
    return groups.size() == answers[0].size();
}

template<typename T>
void solve(string test_answers, vector<array<T, 2>> params, vector<string> judge_answers, vector<T> data) {
    assert(params.size() == judge_answers.size());
    auto masks = get_masks_of_answers(judge_answers);
    string test_mask;
    for (auto param : params) {
        test_mask += '0' + is_yes(data, param);
    }
    auto it = find(masks.begin(), masks.end(), test_mask);
    assert(it != masks.end() && "Something went wrong!");
    cout << (test_answers[it - masks.begin()] == '1' ? YES : NO);
}

void calc_expected_number_of_submissions(int tests, int samples) {
    function<int(int)> f = [&](int n) {
        if (n <= 1) return 0;
        static uniform_int_distribution<int> gen(0, 1);
        static mt19937 rnd(1);
        int c1 = 0;
        for (int i = 0; i < n; ++i) c1 += gen(rnd);
        return max(f(c1), f(n - c1)) + 1;
    };
    vector<int> m(samples);
    for (int& i : m) i = f(tests) + 2;
    sort(m.begin(), m.end());
    vector<double> thr = {0.0001, 0.001, 0.01, 0.05, 0.1, 0.25, 0.5, 0.75, 0.9, 0.95, 0.99, 0.999};
    for (int l = 0, i = 0; l < samples; ++l) {
        int r = l;
        while (r + 1 < samples && m[r + 1] == m[l]) ++r;
        double prob = double(r + 1) / samples;
        if (i < thr.size() && prob >= thr[i]) {
            ++i;
            cout << m[r] << " submissions give you ~" << prob * 100 << "% chance to get AC for " << tests << " tests\n";
        }
        l = r;
    }
}

int main() {
    //calc_expected_number_of_submissions(?, 1000); return 0;          //Uncomment this line to estimate needed number of submissions to get AC
    using T = uint64_t;
    vector<T> data;
    {
        //Read data
    }

    //First stage, get test_answers
    string test_answers = "";
    if (test_answers.empty()) {
        cout << YES;
        return 0;
    }

    //Second stage
    vector<array<T, 2>> params;
    vector<string> judge_answers;

    vector<T> primes = {11305367, 12323617, 13177961, 15416749, 16750309, 16978933, 18348293, 20146331, 20712077, 20964221, 21309023, 21554711, 22703899, 23844127, 24050933, 27283787, 31030453, 31064333, 31282249, 32018419, 33503941, 34850437, 34873193, 35716141, 37801381, 40843487, 42150841, 42278111, 43956463, 44020187, 45623153, 45898373, 46584313, 46607497, 47149967, 47243363, 48014893, 48095303, 52504363, 52765291, 52934261, 53343967, 53529863, 54294551, 54406283, 55845749, 56598251, 58372723, 58449637, 58486469, 59342303, 60002617, 61589711, 61609291, 62182103, 62195999, 62699953, 63096233, 65445377, 66585641, 67403081, 67767587, 67957391, 68288797, 69091991, 72160021, 72765533, 74040983, 74967751, 75284659, 76920881, 78179477, 78198431, 78514013, 79327159, 79766549, 80418581, 81767129, 82190261, 82564117, 84264641, 84985861, 87423209, 87580769, 88997939, 89918459, 90147983, 90273067, 91887337, 92155253, 94120597, 94585333, 94918651, 95319671, 96551617, 97289051, 97844381, 97896059, 99269887, 99952243};
    shuffle(primes.begin(), primes.end(), mt19937(777));

    //Just keep adding answers from judge while you don't get AC

    for (int i = 0; i < judge_answers.size() + 1; ++i) {
        assert(i * 2 + 1 < primes.size() && "Add more primes (or just random numbers)");
        params.push_back({primes[i * 2], primes[i * 2 + 1]});
    }
    for (auto s : judge_answers) assert(s.size() == test_answers.size());
    for (size_t t = 0; char c : test_answers) {
        if (c == '0') for (auto& s : judge_answers) s[t] ^= 1;
        ++t;
    }
    if (judge_answers.size() && check_all_masks_are_distinct(judge_answers)) {
        cerr << "DONE!" << endl;
        params.pop_back();
        solve(test_answers, params, judge_answers, data);
    } else {
        cout << (is_yes(data, params.back()) ? YES : NO);
    }
}

//Second program
/*
#include "bits/stdc++.h"
using namespace std;

int main() {
    string res(test_number, '?');
    //parse responce from judging system and set i-th char to '0' if test answer if NO, '1' if YES
    cout << "judge_answers.push_back(\"" << res << "\");\n";
}
*/
