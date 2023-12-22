#include "Syntax/CFGInputParser.h"
#include "Syntax/CFGElement.h"

CFGInputParser::CFGInputParser(string inputFilePath){
    parse(inputFilePath);
}

map<string, CFGElement*> CFGInputParser::getCFGElements(){
    return cfg;
}

CFGElement* CFGInputParser::getStartSymbol(){
    return startSymbol;
}

void CFGInputParser::constructProdsList(){
    for(auto prod: productions){
        string name = prod.first;
        CFGElement* element = cfg[name];

        for(auto str: prod.second){

            if(str == "|"){
                element->pushProduction();
            }else{
                if(str[0] == '\'') str = str.substr(1, str.length()-2);

                if(cfg.find(str) != cfg.end()){
                    element->pushElement(cfg[str]);
                }else{
                    CFGElement* newElement = new CFGElement(str, true);
                    element->pushElement(newElement);
                    cfg[str] = newElement;
                }
                if(str == "\\L") element->setHasEpsilon(true);
            }
        }

    }
}

vector<string> splitBySpace(string s) {
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

void CFGInputParser::parse(string inputFilePath){
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Unable to open the file." << endl;
        return;
    }

    string line;
    string prods = "";
    string name = "";
    bool isStart = true;
    
    while (getline(inputFile, line)) {

        line = regex_replace(line, regex("\\|"), " | ");

        regex re("^.+ ::= .+$");

        if(regex_match(line, re)){ // New Non Terminal
            vector<string> proElements = splitBySpace(prods);

            if(name != ""){
                productions[name] = proElements;
                CFGElement* element = new CFGElement(name, false);
                cfg[name] = element;
                if(isStart){ 
                    startSymbol = element;
                    isStart = false;
                }
            }
            
            string delimiter = " ::= ";
            int pos = line.find(delimiter);
            name = line.substr(0, pos);
            int end = line.length() - pos + delimiter.length();
            prods = line.substr(pos + delimiter.length(), end);

        }else{
            prods += line;
        }

    }

    vector<string> proElements = splitBySpace(prods);
    productions[name] = proElements;
    CFGElement* element = new CFGElement(name, false);
    cfg[name] = element;

    constructProdsList();
}
