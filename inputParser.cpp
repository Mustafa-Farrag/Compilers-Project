#include <bits/stdc++.h>
#include <regex>
using namespace std;

class inputParser{
    private:
    map<string, string> regularDefs;
    map<string, string> regularExprs;
    set<string> keywords;
    set<string> punctuations;

    vector<string> split_str(string s, string del) {
        int end = s.find(del);
        vector<string> strs;
        while (end != -1) {
            strs.push_back(s.substr(0, end));
            s.erase(s.begin(), s.begin() + end + 1);
            end = s.find(del);
        }
        strs.push_back(s.substr(0, end));
        return strs;
    }

    void parse(string inputFilePath){
        ifstream inputFile(inputFilePath);
        if (!inputFile.is_open()) {
            cerr << "Unable to open the file." << endl;
            return;
        }

        string line;
        
        while (getline(inputFile, line)) {
            line = regex_replace(line, regex("\\\\(?!L)"), "");
            
            if(line[0] == '{'){ // Keyword Line
                regex re("[a-zA-Z]+"); // Keywords
                smatch match;

                while (regex_search(line, match, re)) {
                    keywords.insert(match[0]);
                    line = match.suffix();
                }

                continue;
            }

            if(line[0] == '['){ // Punctuations Line
                line = line.substr(1, line.size()-2);

                vector<string> strs = split_str(line, " ");
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
    public:
    inputParser(string inputFilePath){
        parse(inputFilePath);
    }
    map<string, string> getRD(){
        return regularDefs;
    }
    map<string, string> getRE(){
        return regularExprs;
    }
    set<string> getKeywords(){
        return keywords;
    }
    set<string> getPuncts(){
        return punctuations;
    }
};

int main(){
    inputParser inParser("input.txt");
    set<string> keywords = inParser.getKeywords();
    set<string> puncts = inParser.getPuncts();
    map<string, string> rd = inParser.getRD();
    map<string, string> re = inParser.getRE();
    cout << "Hi";
    return 0;
}

