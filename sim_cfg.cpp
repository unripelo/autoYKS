#include <bits/stdc++.h>
using namespace std;

// Simple top-down recursive parser for grammar:
// S -> 'a' S 'b' | ""
// Language: { a^n b^n | n >= 0 }

bool parseS(const string &s, int &pos) {
    // try to match 'a' S 'b' repeatedly
    if (pos < (int)s.size() && s[pos] == 'a') {
        pos++; // consume a
        if (!parseS(s, pos)) return false;
        if (pos < (int)s.size() && s[pos] == 'b') { pos++; return true; }
        return false;
    }
    // epsilon
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << "CFG simulator (S -> a S b | epsilon). Enter a string:" << flush << '\n';
    string s; if (!(cin >> s)) return 0;
    int pos = 0;
    bool ok = parseS(s, pos) && pos == (int)s.size();
    cout << (ok ? "ACCEPT" : "REJECT") << '\n';
}
