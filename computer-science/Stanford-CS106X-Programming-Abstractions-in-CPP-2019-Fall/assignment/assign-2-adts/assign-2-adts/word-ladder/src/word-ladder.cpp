/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"

#include <map>
#include <queue>
#include <set>

static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "\"." << endl;

    queue<string> next; // queue for BSF
    next.push(start);
    set<string> visited; // keep track of whether a word has been visited before
    map<string, string> prev; // keep track of the previous word
    prev.emplace(start, string());

    while (true) {
        string word = next.front();
        next.pop();
        visited.insert(word);

        for (string::size_type i = 0; i < word.size(); ++i) {
            string next_word = word;
            for (char j = 'a'; j <= 'z'; ++j) {
                next_word[i] = j;
                if (next_word == end) {
                    prev.emplace(end, word);

                    // find the ladder
                    vector<string> answer;
                    auto curr = end;
                    while (!curr.empty()) {
                        answer.push_back(curr);
                        curr = prev[curr];
                    }
                    cout << "Found ladder: ";
                    for (auto riter = answer.rbegin(); riter != answer.rend(); ++riter){
                        cout << *riter << " ";
                    }
                    cout << endl << endl;

                    return;
                }
                if (english.contains(next_word) && visited.find(next_word) == visited.end()){
                    next.push(next_word);
                    prev.emplace(next_word, word);
                }
            }
        }
    }
}

static const string kEnglishLanguageDatafile = "../word-ladder/res/dictionary.txt";
static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
        if (end.empty()) break;
        generateLadder(english, start, end);
    }
}

int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
