#include "headers/DFAHandler.h"
#include "headers/NFA.h"
#include "headers/State.h"
#include "headers/Transition.h"
#include <vector>
#include <map>
#include <stack>

DFAHandler :: DFAHandler(map<int, State*> idStatesMap){
    idStatesMap2 = idStatesMap;
    map<string, set<int>> nStates_To_NStates; 
}

DFAHandler :: ~DFAHandler(){
    for (auto& pair : idStatesMap2) {
        delete pair.second; 
    }
    idStatesMap2.clear();  

    nStates_To_NStates.clear();
    DFATransitionTable.clear();
    acceptanceStateToClassType.clear();
}
map<string, map<string, string>> DFAHandler::getDFATransitionTable(){
    return DFATransitionTable;
}


map<string, map<string, string>> DFAHandler :: ConstructDFATransitionTable(   map<int, map<string, vector<int>>> NFATransistionTable, State* startState){
    map<string, map<string, string>> DFATransitionTable;
    // map<string, set<int>> sState_To_NStates;

    map<string, vector<int>> transStartState = NFATransistionTable.at(startState->getNum());


    vector<int> epsClosureStates = transStartState.at("\\L");

    epsClosureStates.insert(epsClosureStates.begin(), startState->getNum());


    std::sort(epsClosureStates.begin(), epsClosureStates.end());

    set<int> epsClosureStatesSorted(epsClosureStates.begin(), epsClosureStates.end());
    // std::sort(epsClosureStatesSorted.begin(), epsClosureStatesSorted.end());

    string concatenatedString = getConcatenatedString(epsClosureStatesSorted);


    nStates_To_NStates.insert(pair<string, set<int>>(concatenatedString, epsClosureStatesSorted));


    stack<set<int>> remainingStates;
    set<string> doneStates;
    remainingStates.push(epsClosureStatesSorted);

    while(!remainingStates.empty()){
        
        set<int> curr = remainingStates.top();
        remainingStates.pop();
        if(doneStates.find(getConcatenatedString(curr)) != doneStates.end()){
            // cout<<"done before"<<endl;
            continue;
        }

        doneStates.insert(getConcatenatedString(curr));


        set<string> inputs = getAllInputs(curr);
        // cout<<"iam here2"<<endl;


        map<string, string> inputTransPair;
        

        for(auto input: inputs){
            // vector<int> nextStates;
            string nextStatesStr;
            set<int> nextStatesids;

            
            for(auto id: curr){
                State* s = idStatesMap2.at(id);

                vector<State*> nextStates =  s->applyInput(input);
                set<int> nextStatesidsTemp = get_ids(nextStates);
                nextStatesids.insert(nextStatesidsTemp.begin(), nextStatesidsTemp.end());

                vector<State*> nextStatesEps =  s->getEpsilonStates();
                set<int> nextStatesidsEpsTemp = get_ids(nextStatesEps);
                nextStatesids.insert(nextStatesidsEpsTemp.begin(), nextStatesidsEpsTemp.end());
            }
            vector<int> tempVec(nextStatesids.begin(), nextStatesids.end());
            std::sort(tempVec.begin(), tempVec.end());
            set<int> nextStatesidsProcessed(tempVec.begin(), tempVec.end());
            nextStatesStr = getConcatenatedString(nextStatesidsProcessed);
            nStates_To_NStates.insert(pair<string, set<int>>(nextStatesStr, nextStatesidsProcessed));
            inputTransPair.insert(pair<string, string>(input, nextStatesStr));



            remainingStates.push(nextStatesidsProcessed);

        }

        string currStateStr = getConcatenatedString(curr);
        DFATransitionTable.insert(pair<string, map<string, string>>(currStateStr, inputTransPair));

    }
    this->DFATransitionTable = DFATransitionTable;
    return DFATransitionTable;
}

string DFAHandler :: getConcatenatedString(set<int> states){
    string concatenatedString = "";
    bool accept = false;
    bool start = false;
    string type;
    for (auto id: states) {
        concatenatedString += to_string(id);
        if(idStatesMap2[id]->getIsAccept()){
            accept = true;
            type = idStatesMap2[id]->getClassType();
        }
        if(idStatesMap2[id]->getIsStart()){
            start = true;
        }
    }
    if(accept){
        acceptanceStateToClassType[concatenatedString] = type;
    }
    if(start){
        startState = concatenatedString;
    }
    
    return concatenatedString;
}

set<string> DFAHandler :: getAllInputs(set<int> states){
    set<string> inputs;
    for(auto id: states) {
        // cout<<to_string(id)<<endl;
        State* curr = idStatesMap2.at(id);
        

        vector<Transition*> trans = curr->getTransitions(); 
        for (int j=0; j<trans.size(); j++){
            inputs.insert(trans[j]->getConditionStr());
        }
    }
    return inputs;
}

map<string, string>  DFAHandler :: getAcceptanceStateToClassType(){
    return acceptanceStateToClassType;
}

string DFAHandler :: getStartState(){
    return startState;
}

set<int> DFAHandler :: get_ids(vector<State*> nextStates){
    set<int> ids;
    for (auto state: nextStates){
        ids.insert(state->getNum());
    }
    return ids;
}

map<string, set<int>>  DFAHandler::getnStates_To_NStates(){
    return nStates_To_NStates;
}

map<int, State*> DFAHandler::getidStatesMap(){
    return idStatesMap2;
}


// vector<int> DFAHandler :: get_ids(vector<State*> nextStates){
//     vector<int> ids;
//     for (auto state: nextStates){
//         ids.push_back(state->getNum());
//     }
//     return ids;
// }
 