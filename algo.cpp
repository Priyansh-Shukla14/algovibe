#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>
#include <algorithm>
using namespace std;

// Recursive parser that handles parentheses and counts
unordered_map<string, int> parseFormula(const string &s, int &i) {
    unordered_map<string, int> counts;

    while (i < s.size()) {
        if (s[i] == '(') {
            i++; // skip '('
            auto inner = parseFormula(s, i); // parse inside
            i++; // skip ')'

            // read multiplier
            int num = 0;
            while (i < s.size() && isdigit(s[i])) {
                num = num * 10 + (s[i] - '0');
                i++;
            }
            if (num == 0) num = 1;

            // multiply inner elements
            for (auto &p : inner) counts[p.first] += p.second * num;
        } 
        else if (s[i] == ')') break;
        else if (isupper(s[i])) {
            string elem;
            elem += s[i++];
            // element can have lowercase (not in this problem, but general)
            while (i < s.size() && islower(s[i])) elem += s[i++];

            int num = 0;
            while (i < s.size() && isdigit(s[i])) {
                num = num * 10 + (s[i] - '0');
                i++;
            }
            if (num == 0) num = 1;

            counts[elem] += num;
        } 
        else if (s[i] == ',' || s[i] == ' ') {
            i++;
        } 
        else break;
    }

    return counts;
}

// Compare element counts
bool compareFormulas(unordered_map<string,int> &a, unordered_map<string,int> &b, string &msg) {
    bool equal = true;
    for (auto &p : a) {
        if (b[p.first] < p.second) {
            msg = "Missing " + p.first;
            equal = false;
            break;
        }
    }
    for (auto &p : b) {
        if (a[p.first] < p.second) {
            msg = "Extra " + p.first;
            equal = false;
            break;
        }
    }
    return equal;
}

int main() {
    int N;
    cin >> N;
    cin.ignore();

    while (N--) {
        string formula;
        getline(cin, formula);

        int arrow = formula.find("->");
        string left = formula.substr(0, arrow);
        string right = formula.substr(arrow + 2);

        int i = 0;
        auto leftCount = parseFormula(left, i);
        i = 0;
        auto rightCount = parseFormula(right, i);

        string msg;
        if (compareFormulas(leftCount, rightCount, msg))
            cout << "EQUIVALENT\n";
        else
            cout << "NOT_EQUIVALENT: " << msg << "\n";
    }
}
