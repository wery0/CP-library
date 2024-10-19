mutex mtx;
void common() {

}

class TestCase {
    int num;
    double weight = 1;
    stringstream out;

public:
    TestCase(int num): num(num) {}
    double get_weight() const {return weight;}
    int get_number() const {return num;}
    string get_output() const {return out.str();}

    void read_input() {

    }

    void solve() {

    }
};

int main() {
    cin.tie(0), cout.tie(0), cin.sync_with_stdio(0), cout.sync_with_stdio(0);
    common();
    int T; cin >> T;
    vector<TestCase*> tests(T);
    double W = 0;
    for (int i = 0; i < T; ++i) {
        tests[i] = new TestCase(i);
        tests[i]->read_input();
        W += tests[i]->get_weight();
    }
    vector<thread> threads(std::thread::hardware_concurrency());
    assert(threads.size() == std::thread::hardware_concurrency());
    cerr << "Threads = " << threads.size() << endl;
    for (auto& thr : threads) {
        static auto st = std::chrono::steady_clock::now();
        static double w = 0;
        static atomic<int> tc = 0;
        thr = thread([&]() {
            while (tc < T) {
                int cur = tc++;
                tests[cur]->solve();
                mtx.lock();
                double pw = w;
                if (w += tests[cur]->get_weight() / W; int(w * 100 + 1e-9) > int(pw * 100 + 1e-9)) {
                    cerr << "Done " << w * 100 << "% in " << chrono::duration_cast<chrono::milliseconds>(std::chrono::steady_clock::now() - st).count() / 1e3 << "s" << endl;
                }
                mtx.unlock();
            }
        });
    }
    for (auto& thr : threads) thr.join();
    for (int i = 0; i < T; ++i) {
        cout << "Case #" << i + 1 << ": " << tests[i]->get_output() << '\n';
    }
}
