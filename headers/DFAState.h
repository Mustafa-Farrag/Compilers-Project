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

    unordered_map<string, DFAState*> getTransitions(){
        return transitions;
    }

    DFAState* getNextState(string input) {
        // cout << input << " kkkkk\n";
        for (const auto& transition : transitions) {
            regex reg (transition.first);
            // if(input == ";"){
            //     string t = transition.first;
            //     regex reg ("[;]");
            //     cout << transition.first <<  "  " << (transition.first == "[;]") << "  " << regex_search(input, reg) << "\n";
            // }
            if (regex_search(input, reg)) {
                return transition.second;
            }
        }

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