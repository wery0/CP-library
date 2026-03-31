//O(n) on random data
template<typename I>
void bucket_sort(I first, I last) {
    const int n = distance(first, last);
    if (n <= 1) return;
    auto [imne, imxe] = minmax_element(first, last);
    auto mne = *imne, mxe = *imxe;
    if (mne == mxe) return;
    if (n < 32) {
        sort(first, last);
        return;
    }
    int avg = __lg(1 + (mxe - mne) / n);
    while (((mxe - mne) >> avg) > n / 2) ++avg;
    int bl = ((mxe - mne) >> avg) + 1;
    auto get_block = [&](auto x) {return (x - mne) >> avg;};
    static vector<int> cnt, oc, wtf;
    if (cnt.size() < bl + 1) cnt.resize(bl + 1), oc.resize(bl + 1);
    fill(cnt.begin(), cnt.begin() + bl + 1, 0);
    for (auto it = first; it != last; ++it) ++cnt[get_block(*it) + 1];
    for (int i = 1; i < bl + 1; ++i) wtf.push_back(cnt[i]);
    partial_sum(cnt.begin(), cnt.begin() + bl + 1, cnt.begin());
    copy(cnt.begin(), cnt.begin() + bl + 1, oc.begin());
    int i = 0, nb = 1, eso = oc[1];
    for (auto it = first; ; ++it, ++i) {
        if (eso-- == 0) {
            if (it == last) break;
            for (++nb; oc[nb] == oc[nb - 1];) ++nb; eso = oc[nb] - oc[nb - 1] - 1;
            if (cnt[nb - 1] > oc[nb - 1]) {
                int d = cnt[nb - 1] - oc[nb - 1] - 1;
                eso -= d;
                i += d;
                it += d;
            }
        }
        for (; ;) {
            int b = get_block(*it);
            if (b == nb - 1) break;
            auto tmp = *it;
            *it = *(first + cnt[b]);
            *(first + cnt[b]) = tmp;
            ++cnt[b];
        }
    }
    for (int r = n, i = bl - 1; i >= 0; --i) {
        bucket_sort(first + r - wtf.back(), first + r);
        r -= wtf.back();
        wtf.pop_back();
    }
}
