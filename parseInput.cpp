#include <bits/stdc++.h>
#include <regex>
using namespace std;

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

int main(){
    ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        cerr << "Unable to open the file." << endl;
        return 1;
    }

    string line;
    map<string, string> regularDefs;
    map<string, string> regularExprs;

    set<string> keywords;
    set<string> punctuations;
    
    while (getline(inputFile, line)) {
        line = regex_replace(line, regex("\\\\(?!L)"), "");

        if(line[0] == '{'){ // Keyword Line
            regex re("[a-zA-Z]+"); // Keywords
            smatch match;
            cout << "Keyword Line found: ";

            while (regex_search(line, match, re)) {
                cout << match[0] << "     ";
                keywords.insert(match[0]);
                line = match.suffix();
            }

            cout << endl;
            continue;
        }

        if(line[0] == '['){ // Punctuations Line
            cout << "Punctuations Line found: ";

            line = line.substr(1, line.size()-2);

            vector<string> strs = split_str(line, " ");
            for(auto str: strs){
                cout << str << "     ";
                punctuations.insert(str);
            }

            cout << endl;
            continue;
        }

        regex re("^[a-zA-Z]+ = .+$"); // Regular definitions
        smatch match;

        if (regex_search(line, match, re)) {
            cout << "Regular definitions found: " << line << endl;
            int idx = line.find('=');
            
            regularDefs[line.substr(0, idx - 1)] = line.substr(idx + 2);

        } else { // Regular expression
            cout << "Regular expressions found: " << line << endl;
            int idx = line.find(':');
            
            regularExprs[line.substr(0, idx)] = line.substr(idx + 2);
        }
    }

    inputFile.close();
    return 0;
}

