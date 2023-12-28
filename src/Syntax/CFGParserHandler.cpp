#include "Syntax/CFGInputParser.h"
#include "Syntax/CFGElement.h"
#include "Syntax/CFGParserHandler.h"
#include "Syntax/CFGFirstFollowEvaluator.h"

CFGParserHandler::CFGParserHandler(CFGFirstFollowEvaluator* _FFEvaluator, map<string, CFGElement*> cfgElements){
    FFEvaluator = _FFEvaluator;
    this->cfgElements = cfgElements;
    constructParseTable();
}

map<string, map<string, vector<string>>> CFGParserHandler::getParserTable(){
    return parserTable;
}

void CFGParserHandler::constructParseTable(){
    map<CFGElement*, set<string>> firstSets = FFEvaluator->getFirstSet();
    map<CFGElement*, set<string>> followSets = FFEvaluator->getFollowSet();

    cout << "----------- First Sets -----------" << endl;

    for(auto entry: firstSets){
        cout << entry.first->getName() << ": ";
        cout << "[ ";
        for(auto f: entry.second){
            cout << f << ' ';
        }
        cout << "]" << endl;
    }

    cout << endl;
    cout << "----------- Follow Sets -----------" << endl;

    for(auto entry: followSets){
        cout << entry.first->getName() << ": ";
        cout << "[ ";
        for(auto f: entry.second){
            cout << f << " ";
        }
        cout << "]" << endl;
    }

    cout << endl;

    map<string, CFGElement*> elements = FFEvaluator->getElements();

    vector<string> cols;
    vector<string> rows;

    for(auto& pair : elements){
        string key = pair.first;
        CFGElement* element = pair.second;

        if (element->getIsTerminal()){
            if (key == "\\L") continue;
            cols.push_back(key);
        }else{
            rows.push_back(key);
        }
    } 

    cols.push_back("$");

    for(string row: rows){

        CFGElement* ele = elements[row];
        set<string> firstSet = firstSets[ele];
        set<string> followSet = followSets[ele];

        bool firstContainEpsilon = false;

        if (firstSet.find("\\L") != firstSet.end()){
            firstContainEpsilon = true;
        }

        map<string, vector<string>> colsTrans;

        for(string col: cols){  
            vector<string> trans;
            if (col == "$"){
                if (firstContainEpsilon){
                    trans.push_back("\\L");
                }else {
                    trans.push_back("sync");
                }
                colsTrans.insert(pair<string, vector<string>>(col, trans));

            } else {
                if (firstSet.find(col) != firstSet.end()){
                    trans = getSuitableTransition(row, col, elements, firstSets);

                }else if (followSet.find(col) != followSet.end()){
                    if (firstContainEpsilon){
                        trans.push_back("\\L");
                    }else{
                        trans.push_back("sync");
                    }

                }else {
                    trans.push_back("");

                }
                colsTrans.insert(pair<string, vector<string>>(col, trans));

            }
        }
        this->parserTable.insert(pair<string, map<string, vector<string>>>(row, colsTrans));
    }
}

vector<string> CFGParserHandler::getSuitableTransition(string row, string col, 
                                                        map<string, CFGElement*> elements, 
                                                        map<CFGElement*, set<string>> firstSets){
    vector<string> trans;

    CFGElement* ele = elements[row];
    vector<vector<CFGElement*>> prods = ele->getProductions();

    if (prods.size() == 1){
        vector<CFGElement*> prod = prods.front();
        for (CFGElement* elem : prod){
            trans.push_back(elem->getName());
        }

    }else{
        for (vector<CFGElement*> prod: prods){
            CFGElement* elem = prod.front();
            string elemName = elem->getName();

            if (elem->getIsTerminal() && elemName == col){
                for (CFGElement* eleme : prod){
                    trans.push_back(eleme->getName());
                }

            }else if (!elem->getIsTerminal()){
                set<string> firstSet = firstSets[elem];
                if (firstSet.find(col) != firstSet.end()){
                    for (CFGElement* eleme : prod){
                        trans.push_back(eleme->getName());
                    }

                } 

            }
        }
    }

    return trans;
}


void CFGParserHandler::outputParserTable(string outputfilePath) {

    ofstream outputFile(outputfilePath);

    if (!outputFile.is_open()) 
        std::cerr << "Error opening the file!" << std::endl;

    outputFile << left << setw(110) << setfill(' ') << "Non-Temrinal \\ Terminal";

    vector<string> cols;

    for (auto element : cfgElements){
        if (element.second->getIsTerminal()){
             if (element.second->getName() == "\\L") continue;
             outputFile << left << setw(110) << setfill(' ') << element.second->getName();
             cols.push_back(element.second->getName());
        }
           
    }

    outputFile << endl;

    for (auto element : cfgElements){
        if (!element.second->getIsTerminal()){
            if (element.second->getName() == "ASSIGNMENT") cout << "Fuck";
            outputFile << left << setw(110) << setfill(' ') << element.second->getName();
            map<string, vector<string>> elements = parserTable[element.second->getName()];
            for (auto col : cols){
                vector<string> rule = elements[col];
                string ruleString = "";
                for (auto ele : rule)
                    ruleString = ruleString + " -> " + ele;
                if (ruleString != " -> " && ruleString != " -> sync")
                    outputFile << left << setw(110) << setfill(' ') << ruleString;
                else if (ruleString == " -> ")
                    outputFile << left << setw(110) << setfill(' ') << "Error";
                else
                     outputFile << left << setw(110) << setfill(' ') << "Sync";
            }
        }

        else continue;
           
        outputFile << endl;
    }

    outputFile.close();

}
