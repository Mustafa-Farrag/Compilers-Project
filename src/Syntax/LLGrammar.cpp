#include "Syntax/LLGrammar.h"

LLGrammar::LLGrammar(){

}

vector<string> splitBySpaces(string s) {
    regex startSpace("^\\s");
    regex endSpace("\\s$");
    regex spaces("\\s+");
    
    s = regex_replace(s, spaces, " ");
    s = regex_replace(s, startSpace, "");
    s = regex_replace(s, endSpace, "");

    int end = s.find(" ");
    vector<std::string> strs;
    while (end != -1) {
        strs.push_back(s.substr(0, end));
        s.erase(s.begin(), s.begin() + end + 1);
        end = s.find(" ");
    }
    strs.push_back(s.substr(0, end));

    return strs;
}

void LLGrammar::eliminateLR(string nonTerminal){
    string name = nonTerminal;
    string newName = nonTerminal + "'";
    vector<vector<string>> prod = productions[name];
    vector<vector<string>> inNewRule;
    vector<vector<string>> inOldRule;

    for(int i=0;i<prod.size();i++){
        if(prod[i][0] == name){
            vector<string> v ;
            for(int j=1;j<prod[i].size();j++){
                v.push_back(prod[i][j]);
            }
            inNewRule.push_back(v);
        }else{
            inOldRule.push_back(prod[i]);
        }
    }

    if(inNewRule.size() == 0){
        return;
    }

    for (auto &v : inOldRule){
        v.push_back(newName);
    }
 
    for (auto &v : inNewRule){
        v.push_back(newName);
    }
    vector<string> ep = {"\\L"};
    
    inNewRule.push_back(ep);
    
    productions[name] = inOldRule;
    productions[newName] = inNewRule;
    
    cout<<"KK";
}

void LLGrammar:: replaceProduction(string a1, string a2){
 
    vector<vector<string>> rulesA, rulesB, newRulesA;
    rulesA = productions[a1];
    rulesB = productions[a2];

    for (auto rule : rulesA) {
        if (rule[0] == a2) {
            rule.erase(rule.begin());


            for (auto rule1 : rulesB){
                vector<string> c;
                c.reserve(rule.size() + rule1.size());
                c.insert(c.end(), rule1.begin(), rule1.end());
                c.insert(c.end(), rule.begin(), rule.end());
                newRulesA.push_back(c);
            }
        }
        else{
            newRulesA.push_back(rule);
        }
    }
    productions[a1] = newRulesA;
}

void LLGrammar::removeLeftRecursion(){
    vector<string> keys;

    // Extract keys from the map
    for (const auto& pair : productions) {
        keys.push_back(pair.first);
    }

    for(int i=0;i<keys.size();i++){
        for(int j = 0; j < i; j++){
            replaceProduction(keys[i], keys[j]);
        }
        eliminateLR(keys[i]);
    }
}

void pushbackToProductions(string name,vector<string> &proElements 
,map<string, vector<vector<string>>>& productions){
    vector<string> rule;
    for(string r : proElements){
        if(r == "|"){
            productions[name].push_back(rule);
            rule.clear();
        }else{
            rule.push_back(r);
        }
    }
    productions[name].push_back(rule);
}

void LLGrammar::parse(string inputFilePath){


    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Unable to open the file." << endl;
        return;
    }

    map<string, vector<vector<string>>> productions;
    string line;
    string prods = "";
    string name = "";
    bool isStart = true;
    
    while (getline(inputFile, line)) {

        line = regex_replace(line, regex("\\|"), " | ");

        regex re("^.+ = .+$");

        if(regex_match(line, re)){ // New Non Terminal
            vector<string> proElements = splitBySpaces(prods);

            if(name != ""){
               pushbackToProductions(name, proElements, productions);
            }
            
            string delimiter = " = ";
            int pos = line.find(delimiter);
            name = line.substr(0, pos);
            int end = line.length() - pos + delimiter.length();
            prods = line.substr(pos + delimiter.length(), end);

        }else{
            prods += line;
        }

    }
    vector<string> proElements = splitBySpaces(prods);
    pushbackToProductions(name, proElements, productions);
    this->productions = productions;
    removeLeftRecursion();
    cout << "here";
}