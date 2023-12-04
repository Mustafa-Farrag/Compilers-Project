#pragma once

#include <bits/stdc++.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;

// Class representing a state in the DFA
class DFAState {

private:
    string id;  
    bool isAccepting;  
    string classType;
    unordered_map<string, DFAState*> transitions;


public:
    DFAState(string id, bool isAccepting = false, string classType = "") : id(move(id)), isAccepting(isAccepting), classType(classType){}

    void setTransition(string i, DFAState* nextState) {
        transitions[i] = nextState;
    }

    DFAState* getNextState(string input) {
        for (const auto& transition : transitions) {
            regex i (transition.first);
            if (regex_match(input, i)) {
                cout << "Matches regex condition:  "<<  transition.first << std::endl;
                return transition.second;
            }
        }

        // if (transitions.find(input) != transitions.end()) {
        //     return transitions[input];
        // }
        return nullptr; // If no transition exists for the input symbol
    }

    bool getIsAccepting() {
        return isAccepting;
    }

    string getid() {
        return id;
    }

    void setId(string id1){
        id = id1;
    }

    void setAccepting(bool a){
        isAccepting = a;
    }

    void setClassType(string classtype) {
        classType = classtype;
    }

    string getClassType() {
        return classType;
    }
};

// int main() {
//     // Create states for the DFA
//     State* q0 = new State("q0");
//     State* q1 = new State("q1");
//     State* q2 = new State("q2", true); // Accepting state

//     // Set transitions for states
//     q0->setTransition('0', q1);
//     q0->setTransition('1', q0);

//     q1->setTransition('0', q2);
//     q1->setTransition('1', q1);

//     q2->setTransition('0', q2);
//     q2->setTransition('1', q2);

//     // Create the DFA and set the start state
//     DFA dfa;
//     dfa.setStartState(q0);

//     // Test input strings for acceptance
//     vector<string> inputs = {"0110", "1001", "1010", "1100"};

//     for (const auto& input : inputs) {
//         if (dfa.isAccepted(input)) {
//             cout << "Input '" << input << "' is accepted by the DFA." << endl;
//         } else {
//             cout << "Input '" << input << "' is not accepted by the DFA." << endl;
//         }
//     }

//     // Cleanup: Free allocated memory
//     delete q0;
//     delete q1;
//     delete q2;

//     return 0;
// }
