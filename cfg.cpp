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

// Build leftmost derivation steps for grammar S -> a S b | epsilon
// Returns true and fills `steps` when `s` belongs to the language a^n b^n.
bool buildLeftmostDerivation(const string &s, vector<string> &steps) {
    // Check form: some number of 'a's followed by some number of 'b's, nothing else
    int n = (int)s.size();
    int i = 0;
    while (i < n && s[i] == 'a') ++i;
    int a_count = i;
    int b_count = 0;
    while (i < n && s[i] == 'b') { ++i; ++b_count; }
    if (i != n) return false; // contains other characters or mixed order
    if (a_count != b_count) return false; // not same number of a's and b's

    // Build derivation: S -> a S b -> a a S b b -> ... -> a^n b^n
    string cur = "S";
    steps.clear();
    steps.push_back(cur);
    for (int k = 0; k < a_count; ++k) {
        // replace leftmost 'S' with "a S b"
        size_t pos = cur.find('S');
        if (pos == string::npos) return false; // shouldn't happen
        cur = cur.substr(0, pos) + string("a S b") + cur.substr(pos + 1);
        steps.push_back(cur);
    }
    // finally replace S with epsilon (remove it)
    size_t pos = cur.find('S');
    if (pos != string::npos) {
        cur = cur.substr(0, pos) + cur.substr(pos + 1);
        // remove any extra spaces from the removal
        // compact spaces (optional): remove double spaces
        string compact;
        for (char ch : cur) if (ch != ' ' ) compact.push_back(ch);
        cur = compact;
        steps.push_back(cur);
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << "CFG simulator (S -> a S b | epsilon). Enter a string (empty line allowed):" << flush << '\n';
    string s;
    if (!std::getline(cin, s)) return 0;
    // handle CRLF Windows leftover when mixing >> and getline; also allow empty input
    if (!s.empty() && s.back() == '\r') s.pop_back();
    // if the user typed nothing (just pressed Enter) treat as empty string
    int pos = 0;
    bool ok = parseS(s, pos) && pos == (int)s.size();

    if (ok) {
        cout << "ACCEPT\n";
        vector<string> steps;
        if (buildLeftmostDerivation(s, steps)) {
            cout << "Leftmost derivation steps:" << '\n';
            for (size_t i = 0; i < steps.size(); ++i) {
                cout << i << ": " << steps[i] << '\n';
            }
        } else {
            // For safety: if derivation builder failed, at least print a short note
            cout << "(Accepted, but no derivation produced by builder.)" << '\n';
        }
    } else {
        cout << "REJECT" << '\n';
    }
}
