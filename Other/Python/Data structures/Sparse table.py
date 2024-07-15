class SparseTable:
    # <O(nlog(n) * merge()), O(nlog(n))>
    def __init__(self, arr, merge):
        n = len(arr)
        m = [[arr[j] for j in range(max(0, n + 1 - (1 << i)))] for i in range(n.bit_length())]
        for i in range(1, len(m)):
            for j in range(len(m[i])):
                m[i][j] = merge(m[i - 1][j], m[i - 1][j + (1 << i - 1)])
        self.merge = merge
        self.m = m

    # O(merge())
    def query(self, l, r):
        assert 0 <= l <= r < len(self.m[0])
        u = (r - l + 1).bit_length() - 1
        return self.merge(self.m[u][l], self.m[u][r - (1 << u) + 1])

# st = SparseTable(m, lambda x, y: min(x, y))
