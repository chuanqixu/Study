/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <iostream>
#include <fstream>
using namespace std;

#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim

#include <map>
#include <vector>
#include <sstream>
#include <random>

static const string kGrammarsDirectory = "../random-sentence-generator/res/grammars/";
static const string kGrammarFileExtension = ".g";
static string getNormalizedFilename(string filename) {
    string normalizedFileName = kGrammarsDirectory + filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? [<return> to quit]: "));
        if (filename.empty() || isValidGrammarFilename(filename)) return filename;
        cout << "Failed to open the grammar file named \"" << filename << "\". Please try again...." << endl;
    }
}



map<string, vector<string> *> *generate_nonterminal_map(const string &filename) {
    ifstream file_stream(filename);
    if (!file_stream.is_open()) {
        cout << "Failed to open file!" << endl;
        return nullptr;
    }

    auto *nonterminal_map_ptr = new map<string, vector<string> *>();

    while (true){
        // read the nonterminal
        string line;
        do {
            getline(file_stream, line);
            line = trim(line);
        } while (!file_stream.eof() && line[0] != '<');

        if (file_stream.eof()) {
            break;
        }

        string nonterminal = line;

        // read the number of possible productions
        int num_productions;
        getline(file_stream, line);
        istringstream str_stream(line);
        str_stream >> num_productions;

        // create the vector storing the productions
        (*nonterminal_map_ptr)[nonterminal] = new vector<string>();
        for (int i = 0; i < num_productions; ++i) {
            getline(file_stream, line);
            (*nonterminal_map_ptr)[nonterminal]->push_back(line);
        }
    }

    return nonterminal_map_ptr;
}



string expand(const string sentence, map<string, vector<string> *> *nonterminal_map_ptr) {
    // find the first nonterminal
    auto nonterminal_start = sentence.find('<'), nonterminal_end = sentence.find('>');
    // if there is no nonterminal, directly return the sentence
    if (nonterminal_start == string::npos || nonterminal_end == string::npos) {
        return sentence;
    }

    string expanded_sentence;
    string nonterminal = sentence.substr(nonterminal_start, nonterminal_end - nonterminal_start + 1);

    auto definitions_ptr = (*nonterminal_map_ptr)[nonterminal];
    auto production = (*definitions_ptr)[rand() % definitions_ptr->size()];

    // recursively call to expand both the first nonterminal and the following sentence till the end
    expanded_sentence = sentence.substr(0, nonterminal_start) +
                        expand(production, nonterminal_map_ptr) +
                        expand(sentence.substr(nonterminal_end+1), nonterminal_map_ptr);

    return expanded_sentence;
}



void destroy_nonterminal_map(map<string, vector<string> *> *nonterminal_map_ptr) {
    if (!nonterminal_map_ptr) {
        return;
    }

    for (auto &&item : *nonterminal_map_ptr) {
        delete item.second;
    }
    delete nonterminal_map_ptr;
}



void generate_random_sentence(const string &filename) {
    auto nonterminal_map_ptr = generate_nonterminal_map(filename);
    if (!nonterminal_map_ptr) {
        cout << "Failed in generate nonterminal map!" << endl;
        return;
    }

    auto definitions_ptr = (*nonterminal_map_ptr)["<start>"];
    auto production = (*definitions_ptr)[rand() % definitions_ptr->size()];

    cout << expand(production, nonterminal_map_ptr) << endl;

    destroy_nonterminal_map(nonterminal_map_ptr);
}


int main() {
    while (true) {
        string filename = getFileName();
        if (filename.empty()) break;
        cout << "Here's where you read in the \"" << filename << "\" grammar "
             << "and generate three random sentences." << endl;
        cout << "*****************************************" << endl;
        generate_random_sentence(getNormalizedFilename(filename));
        cout << "*****************************************" << endl << endl;
    }

    
    cout << "Thanks for playing!" << endl;
    return 0;
}
