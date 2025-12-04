#include <bits/stdc++.h>
using namespace std;

// Greibach Normal Form (GNF) Simulator
// GNF: All productions are of the form A -> a α
// where a is a terminal and α is a string of zero or more non-terminals
//
// Example Grammar (in GNF):
// S -> a A | b B | c
// A -> c S | a
// B -> b S | b

struct Production {
    char lhs;                    // Left-hand side (non-terminal)
    char terminal;               // Terminal symbol
    string nonterminals;         // Right-hand side non-terminals (may be empty)
};

class GNFSimulator {
private:
    vector<Production> productions;
    set<char> nonTerminals;
    set<char> terminals;
    char startSymbol;

public:
    GNFSimulator(char start) : startSymbol(start) {
        initializeGrammar();
    }

    void initializeGrammar() {
        // Default GNF grammar:
        // S -> a A | b B | c
        // A -> c S | a
        // B -> b S | b
        
        productions.push_back({'S', 'a', "A"});
        productions.push_back({'S', 'b', "B"});
        productions.push_back({'S', 'c', ""});
        
        productions.push_back({'A', 'c', "S"});
        productions.push_back({'A', 'a', ""});
        
        productions.push_back({'B', 'b', "S"});
        productions.push_back({'B', 'b', ""});
        
        nonTerminals = {'S', 'A', 'B'};
        terminals = {'a', 'b', 'c'};
    }

    void printGrammar() const {
        cout << "\n=== GNF Grammar ===" << '\n';
        map<char, vector<pair<char, string>>> grouped;
        
        for (const auto &prod : productions) {
            grouped[prod.lhs].push_back({prod.terminal, prod.nonterminals});
        }
        
        for (const auto &[nt, prods] : grouped) {
            cout << nt << " -> ";
            for (size_t i = 0; i < prods.size(); ++i) {
                cout << prods[i].first;
                if (!prods[i].second.empty()) {
                    cout << prods[i].second;
                }
                if (i < prods.size() - 1) cout << " | ";
            }
            cout << '\n';
        }
        cout << '\n';
    }

    // Parse using recursive descent
    // Returns true if string can be derived from symbol
    bool parse(const string &input, int &pos, char symbol) {
        if (pos > (int)input.size()) return false;
        
        // Try all productions for this symbol
        for (const auto &prod : productions) {
            if (prod.lhs != symbol) continue;
            
            // Try this production: symbol -> terminal nonterminals
            if (pos >= (int)input.size() || input[pos] != prod.terminal) {
                continue;
            }
            
            int savedPos = pos;
            pos++; // consume the terminal
            
            // Parse the remaining non-terminals
            bool success = true;
            for (char nt : prod.nonterminals) {
                if (!parse(input, pos, nt)) {
                    success = false;
                    break;
                }
            }
            
            if (success) {
                return true;
            }
            
            // Backtrack
            pos = savedPos;
        }
        
        return false;
    }

    bool recognize(const string &input) {
        int pos = 0;
        bool result = parse(input, pos, startSymbol);
        return result && pos == (int)input.size();
    }

    void displayTestResults() {
        vector<string> testStrings = {
            "a",          // S -> a A -> a a (accept)
            "ac",         // S -> a A -> a c S -> a c c (accept)
            "ac",         // Test another path
            "b",          // S -> b B -> b b (accept)
            "bab",        // S -> b B -> b b S -> b b a A (accept)
            "abc",        // Should test various paths
            "cc",         // Not in language (reject)
            ""            // Empty string (reject, GNF requires at least 1 terminal)
        };
        
        cout << "\n=== Testing Strings ===" << '\n';
        for (const auto &str : testStrings) {
            bool accepted = recognize(str);
            cout << "\"" << (str.empty() ? "epsilon" : str) << "\" -> " 
                 << (accepted ? "ACCEPT" : "REJECT") << '\n';
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    GNFSimulator gnf('S');
    gnf.printGrammar();
    
    cout << "Enter a string to parse (or 'test' for test cases):" << '\n';
    string input;
    
    getline(cin, input);
    
    if (input == "test") {
        gnf.displayTestResults();
    } else if (!input.empty()) {
        bool result = gnf.recognize(input);
        cout << (result ? "ACCEPT" : "REJECT") << '\n';
    } else {
        cout << "REJECT (empty input)" << '\n';
    }
    
    return 0;
}
