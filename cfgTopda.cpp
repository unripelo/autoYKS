#include <bits/stdc++.h>
using namespace std;

enum State { Q0, Q1, Q2, Q3 };

static string state_name(State s) {
    switch (s) {
        case Q0: return "q0";
        case Q1: return "q1";
        case Q2: return "q2";
        case Q3: return "q3";
        default: return "?";
    }
}`

static string stack_to_string(const stack<char> &st) {

    vector<char> v;
    stack<char> copy = st;
    while (!copy.empty()) { v.push_back(copy.top()); copy.pop(); }
    
    reverse(v.begin(), v.end());
    string out = "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) out += ",";
        out += v[i];
    }
    out += "]";
    return out;
}

bool simulate(const string &s){
    stack<char> st;
    int n = (int)s.size();
    int i = 0;
    int step = 0;
    int countA = 0;
    State state = Q0; // start state

    cout << "PDA trace:\n";
    cout << "step | idx | sym | action   | state->next | stack\n";
    cout << "-----+-----+-----+----------+------------+--------\n";

    while (i < n) {
        ++step;
        char sym = s[i];
        string action = "";
        State next = state;

        if (sym == 'a' && (state == Q0 || state == Q1)) {
            // push A, move to q1
            st.push('A');
            ++countA;
            action = "push A";
            next = Q1;
            ++i;
        } else if (sym == 'b' && (state == Q1 || state == Q2)) {
            // pop A, move to q2
            if (st.empty()) {
                action = "pop fail";
                cout << setw(4) << step << " | " << setw(3) << i << " | " << setw(3) << sym
                     << " | " << setw(8) << action << " | " << setw(4) << (state_name(state) + "->" + state_name(next))
                     << " | " << stack_to_string(st) << "\n";
                return false;
            }
            st.pop();
            action = "pop A";
            next = Q2;
            ++i;
        } else {
            action = "rej(sym)";
            cout << setw(4) << step << " | " << setw(3) << i << " | " << setw(3) << sym
                 << " | " << setw(8) << action << " | " << setw(4) << (state_name(state) + "->" + state_name(next))
                 << " | " << stack_to_string(st) << "\n";
            return false;
        }

        cout << setw(4) << step << " | " << setw(3) << i-1 << " | " << setw(3) << sym
             << " | " << setw(8) << action << " | " << setw(4) << (state_name(state) + "->" + state_name(next))
             << " | " << stack_to_string(st) << "\n";

        state = next;
    }

    // determine final state
    State final_state = (i==n && st.empty() && countA>=1) ? Q3 : state;
    cout << "-----+-----+-----+----------+------------+--------\n";
    cout << "Final | idx=" << i << " | stack=" << stack_to_string(st) << " | state=" << state_name(final_state) << "\n";
    return (i==n && st.empty() && countA>=1);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << "CFG->PDA simulator (S -> a S b | ab). Enter a string:" << flush << '\n';
    string s; if (!(cin >> s)) return 0;
    cout << (simulate(s) ? "ACCEPT" : "REJECT") << '\n';
}
