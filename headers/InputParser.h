#include <bits/stdc++.h>
#include <regex>
using namespace std;

class InputParser{
private:
    map<string, string> regularDefs;
    map<string, string> regularExprs;
    set<string> keywords;
    set<string> punctuations;

    vector<string> split_str(string s, string del);
    void parse(string inputFilePath);

public:
    InputParser(string inputFilePath);
    map<string, string> getRD();
    map<string, string> getRE();
    set<string> getKeywords();
    set<string> getPuncts();
};
