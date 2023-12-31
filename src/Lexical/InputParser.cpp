#include "Lexical/InputParser.h"

vector<string> splitStrBySpace(string s) {
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

void InputParser::parse(string inputFilePath){
    ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        cerr << "Unable to open the file." << endl;
        return;
    }

    string line;
    
    while (getline(inputFile, line)) {
        line = regex_replace(line, regex("\\\\(?![L+|*^])"), "");

        if(line[0] == '{'){ // Keyword Line
            line = line.substr(1, line.size()-2);

            vector<string> strs = splitStrBySpace(line);
            for(auto str: strs){
                keywords.insert(str);
            }

            continue;
        }

        if(line[0] == '['){ // Punctuations Line
            line = line.substr(1, line.size()-2);

            vector<string> strs = splitStrBySpace(line);
            for(auto str: strs){
                punctuations.insert(str);
            }

            continue;
        }

        regex re("^[a-zA-Z]+ = .+$"); // Regular definitions
        smatch match;

        if (regex_search(line, match, re)) {
            int idx = line.find('=');
            regularDefs[line.substr(0, idx - 1)] = line.substr(idx + 2);

        } else { // Regular expression
            int idx = line.find(':');
            regularExprs[line.substr(0, idx)] = line.substr(idx + 2);
        }
    }

    inputFile.close();
}

InputParser::InputParser(string inputFilePath){
    parse(inputFilePath);
}

map<string, string> InputParser::getRD(){
    return regularDefs;
}

map<string, string> InputParser::getRE(){
    return regularExprs;
}

set<string> InputParser::getKeywords(){
    return keywords;
}

set<string> InputParser::getPuncts(){
    return punctuations;
}
