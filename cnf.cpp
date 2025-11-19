#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::string;

// Simple, easy-to-read CNF-assignment simulator.
// For teaching clarity this program recognizes strings of the form a^n b^n (n >= 1).
// It is intentionally small and uses clear identifier names.

bool is_a_n_b_n(const string &s) {
    int len = (int)s.size();
    if (len == 0) return false;
    int i = 0;
    int countA = 0;
    while (i < len && s[i] == 'a') { ++countA; ++i; }
    int countB = 0;
    while (i < len && s[i] == 'b') { ++countB; ++i; }
    if (i != len) return false; // invalid symbol or order
    return countA >= 1 && countA == countB;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "CNF (simple) simulator — enter a string of the form a^n b^n:" << std::flush << '\n';
    string input;
    if (!(cin >> input)) return 0;

    bool accept = is_a_n_b_n(input);
    cout << (accept ? "ACCEPT" : "REJECT") << '\n';
    return 0;
}
