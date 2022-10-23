struct Node {
    int len = 0;
    Node *sl = 0;
    Node *m[26] = {0};

    Node() {}

    Node(int d) {
        len = d;
    }
};

str t;
int cv = 0, cur = 0;
Node *r0 = new Node(0);
Node *r1 = new Node(-1);
Node *lst = r0;

Node* pizduy(Node *n) {
    for (; n != r1; n = n->sl) {
        int pr = cur - 1 - n->len;
        if (pr >= 0 && t[pr] == t[cur]) break;
    }
    return n;
}

void add_char(char c) {
    assert('a' <= c && c <= 'z');
    c -= 'a';
    lst = pizduy(lst);
    if (!lst->m[c]) {
        cv++;
        lst->m[c] = new Node(lst->len + 2);
        lst->m[c]->sl = lst == r1 ? r0 : pizduy(lst->sl)->m[c];
    }
    lst = lst->m[c];
}

int main() {
    fast;
    r0->sl = r1->sl = r1;
    cin >> t;
    vec<int> ans;
    for (int q = 0; q < t.size(); q++) {
        add_char(t[q]);
        ans.pb(cv);
        cur++;
    }
    for (int i : ans) cout << i << " ";
}
