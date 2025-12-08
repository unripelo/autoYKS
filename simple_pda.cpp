#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// Simple PDA Simulator for Language L = { a^n b^n | n >= 1 }
// Logic:
// 1. Start in q0.
// 2. Read 'a', push 'A' to stack, stay in q0.
// 3. If we see 'b' in q0 and stack has 'A', pop 'A', go to q1.
// 4. If we see 'b' in q1 and stack has 'A', pop 'A', stay in q1.
// 5. If input is finished and stack is empty (or just bottom marker), accept.

struct PDA {
    enum State { q0, q1, q2_accept, q_reject };
    
    State currentState;
    stack<char> st;
    vector<string> traceLog;

    string stateToString(State s) {
        switch(s) {
            case q0: return "q0";
            case q1: return "q1";
            case q2_accept: return "q2(acc)";
            case q_reject: return "rej";
        }
        return "???";
    }

    string stackToString() {
        if (st.empty()) return "[]";
        string s = "";
        stack<char> temp = st;
        while(!temp.empty()) {
            s += temp.top();
            temp.pop();
        }
        reverse(s.begin(), s.end()); // Show bottom-to-top or top-to-bottom? Usually top is right or left. 
        // Let's show as string: bottom..top
        return "[" + s + "]";
    }

    void logStep(int step, char inputChar, const string& action) {
        cout << left << setw(5) << step 
             << setw(10) << stateToString(currentState) 
             << setw(10) << (inputChar == 0 ? "EPS" : string(1, inputChar))
             << setw(15) << stackToString() 
             << action << endl;
    }

    bool run(const string& input) {
        currentState = q0;
        while(!st.empty()) st.pop();
        
        // Optional: Push a bottom marker if needed, but for simple a^n b^n we can just check empty.
        // st.push('$'); 

        cout << "PDA Trace:" << endl;
        cout << left << setw(5) << "Step" 
             << setw(10) << "State" 
             << setw(10) << "Input" 
             << setw(15) << "Stack" 
             << "Action" << endl;
        cout << string(60, '-') << endl;

        int step = 0;
        logStep(step, 0, "Start");

        for (size_t i = 0; i < input.length(); ++i) {
            char c = input[i];
            step++;
            string action = "";

            if (currentState == q0) {
                if (c == 'a') {
                    st.push('A');
                    action = "Push A";
                    // Stay in q0
                } else if (c == 'b') {
                    if (!st.empty() && st.top() == 'A') {
                        st.pop();
                        currentState = q1;
                        action = "Pop A, Go to q1";
                    } else {
                        currentState = q_reject;
                        action = "Fail: Unexpected 'b' or empty stack";
                    }
                } else {
                    currentState = q_reject;
                    action = "Fail: Invalid char '" + string(1, c) + "'";
                }
            } else if (currentState == q1) {
                if (c == 'b') {
                    if (!st.empty() && st.top() == 'A') {
                        st.pop();
                        action = "Pop A";
                        // Stay in q1
                    } else {
                        currentState = q_reject;
                        action = "Fail: Stack empty or no 'A'";
                    }
                } else {
                    currentState = q_reject;
                    action = "Fail: Expected 'b' in q1";
                }
            } else {
                currentState = q_reject;
                action = "Fail: Trapped in reject/accept state";
            }

            logStep(step, c, action);
            if (currentState == q_reject) return false;
        }

        // End of input check
        step++;
        if (currentState == q1 && st.empty()) {
            currentState = q2_accept;
            logStep(step, 0, "Input End: Stack Empty -> Accept");
            return true;
        } else {
            string reason = "Input End: ";
            if (currentState == q0) reason += "Stuck in q0 (needs 'b')";
            else if (!st.empty()) reason += "Stack not empty";
            
            currentState = q_reject;
            logStep(step, 0, reason);
            return false;
        }
    }
};

int main() {
    PDA pda;
    string input;
    cout << "Simple PDA Simulator (L = { a^n b^n | n >= 1 })" << endl;
    cout << "Enter a string: ";
    if (cin >> input) {
        bool result = pda.run(input);
        cout << string(60, '-') << endl;
        if (result) cout << "RESULT: ACCEPTED" << endl;
        else cout << "RESULT: REJECTED" << endl;
    }
    return 0;
}
