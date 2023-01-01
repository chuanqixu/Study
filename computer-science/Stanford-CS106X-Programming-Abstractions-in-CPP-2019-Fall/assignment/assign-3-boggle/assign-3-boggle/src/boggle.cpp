/**
 * File: boggle.cpp
 * ----------------
 * Implements the game of Boggle.
 */

#include <cctype>
#include <iostream>
#include <ctype.h>
#include "console.h"
#include "gboggle.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "random.h"
#include "grid.h"
#include "map.h"
#include "set.h"
#include "lexicon.h"

#include <set>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

static const string kStandardCubes[16] = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

static const string kBigBoggleCubes[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

static const int kMinLength = 4;
static const double kDelayBetweenHighlights = 100;
static const double kDelayAfterAllHighlights = 500;

/**
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */
static void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl;
    cout << endl;
}

/**
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */
static void giveInstructions() {
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl;
    cout << endl;
    cout << "Hit return when you're ready...";
    getLine(); // ignore return value
}

/**
 * Function: getPreferredBoardSize
 * -------------------------------
 * Repeatedly prompts the user until he or she responds with one
 * of the two supported Boggle board dimensions.
 */

static int getPreferredBoardSize() {
    cout << "You can choose standard Boggle (4x4 grid) or Big Boggle (5x5 grid)." << endl;
    return getIntegerBetween("Which dimension would you prefer: 4 or 5?", 4, 5);
}




// ***********************************************************************************
// Part 1: construct the letter cubes
string *generate_user_cubes(int dimension) {
    int num_cubes = dimension * dimension;
    auto user_cubes_ptr = new string[num_cubes];
    for (int i = 0; i < num_cubes; ++i) {
        cout << "For cube " << i << ", enter a string from which the letter is chosen: " << endl;
        string cube_str;
        while (true) {
            bool flag = true;
            while (!(cin >> cube_str)) {
                cout << "Reading string failed" << endl;
            }
            for (auto &c : cube_str) {
                if (isdigit(c)) {
                    cout << "Please only enter characters" << endl;
                    break;
                    flag = false;
                }
            }
            if (flag) {
                break;
            }
        }
        user_cubes_ptr[i] = cube_str;
    }
    return user_cubes_ptr;
}

Grid<char> *generate_boggle(int dimension, const string *cubes) {
    int num_cubes = dimension * dimension;

    // create the shuffled grid
    string shuffled_cubes[num_cubes];
    for (int i = 0; i < num_cubes; ++i) {
        shuffled_cubes[i] = cubes[i];
    }
    for (int i = 0; i < num_cubes - 1; ++i) {
        int r = randomInteger(i+1, num_cubes - 1);
        auto temp = shuffled_cubes[i];
        shuffled_cubes[i] = shuffled_cubes[r];
        shuffled_cubes[r] = temp;
    }

    // generate the boggle
    auto boggle_ptr = new Grid<char>(dimension, dimension);
    for (int i = 0; i < num_cubes; ++i) {
        string cube_str = shuffled_cubes[i];
        (*boggle_ptr)[i/dimension][i%dimension] = cube_str[randomInteger(0, cube_str.size() - 1)];
    }
    return boggle_ptr;
}


Grid<char> *prompt_generate_letter_cubes(int dimension) {
    const string *cubes_ptr = dimension == 4 ? kStandardCubes : kBigBoggleCubes;
    Grid<char> *boggle_ptr;
    if (getYesOrNo("Do you want to generate the boggle by yourself?")) {
        cubes_ptr = generate_user_cubes(dimension);
        boggle_ptr = generate_boggle(dimension, cubes_ptr);
        delete cubes_ptr;
    } else {
        boggle_ptr = generate_boggle(dimension, cubes_ptr);
    }
    return boggle_ptr;
}

Grid<char> *draw_init_boggle(int dimension) {
    int num_cubes = dimension * dimension;
    auto boggle_ptr = prompt_generate_letter_cubes(dimension);
    for (int i = 0; i < num_cubes; ++ i) {
        labelCube(i/dimension, i%dimension, (*boggle_ptr)[i/dimension][i%dimension]);
    }
    return boggle_ptr;
}
// **************************************************************************************





// **************************************************************************************
// Part 2: precompute all formable words

void generate_all_words(set<string> *words_ptr, Grid<char> *boggle_ptr, const Lexicon &english, const string &pre, int row, int col, set<int> &visited) {
    if (pre.size() > boggle_ptr->numRows() * boggle_ptr->numCols()) {
        return;
    }

    set<int> visited_curr(visited);
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (boggle_ptr->inBounds(row + i, col + j) && (i != 0 || j != 0)) {
                string next_str = pre + boggle_ptr->get(row + i, col + j);
                int index = (row + i) * boggle_ptr->numRows() + col + j;
                if (visited.find(index) == visited.cend() && english.containsPrefix(next_str)) {
                    visited_curr.insert(index);
                    if (next_str.size() > 3 && english.contains(next_str)) {
                        words_ptr->insert(toLowerCase(next_str));
                    }
                    generate_all_words(words_ptr, boggle_ptr, english, next_str, row + i, col + j, visited_curr);
                    visited_curr.erase(index);
                }
            }
        }
    }
}


set<string> *main_generate_all_words(Grid<char> *boggle_ptr, const Lexicon &english) {
   auto *words_ptr = new set<string>;
    for (int i = 0; i < boggle_ptr->numRows(); ++i) {
        for (int j = 0; j < boggle_ptr->numCols(); ++j) {
            set<int> visited;
            string pre(1, boggle_ptr->get(i, j));
            generate_all_words(words_ptr, boggle_ptr, english, pre, i, j, visited);
        }
    }
    return words_ptr;
}

// **************************************************************************************





// **************************************************************************************
// Part 3: print the words chosen by the user and the computer
void start_game(Grid<char> *boggle_ptr, set<string> *words_ptr, const Lexicon &english) {
    // create a vector containing the characters of the boggle
    // to help highlight the words when the user enters a character
    vector<char> boggle_chars;
    for (int i = 0; i < boggle_ptr->numRows(); ++i) {
        for (int j = 0; j < boggle_ptr->numCols(); ++j) {
            boggle_chars.push_back(toLowerCase(boggle_ptr->get(i, j)));
        }
    }

    set<string> user_words;
    string user_word;
    bool quit = false;

    // The human's turn
    while (true) {
        while (true) {
//            // highlight the characters when the user enters them
//            while (cin >> c) {
//                if (c == '\n') {
//                    break;
//                }
//                c = toLowerCase(c);
//                vector<char>::const_iterator char_pos;
//                while ((char_pos = find(boggle_chars.cbegin(), boggle_chars.cend(), c)) == boggle_chars.cend()) {
//                    cout << "The boggle does not have this character!" << endl;
//                    cout << "Please enter another character" << endl;
//                    cin >> c;
//                }
//                user_word += c;
//                auto char_idx = char_pos - boggle_chars.cbegin();
//                highlightCube(char_idx/5, char_idx%5, true);
//            }
//            if (user_word.empty()) {
//                quit = true;
//                break;
//            }

            user_word.clear();
            cout << "Enter the word you found. Enter a blank line to quit." << endl;
            getline(cin, user_word);

            // dehighlight all characters
            for (int i = 0; i < boggle_ptr->numRows(); ++i) {
                for (int j = 0; j < boggle_ptr->numCols(); ++j) {
                    highlightCube(i, j, false);
                }
            }

            if (user_word == "") {
                quit = true;
                break;
            }

            // prompt if the entered word is not good
            if (words_ptr->find(user_word) == words_ptr->cend()) {
                if (cin.fail()) {
                    cout << "Reading the word failed!" << endl;
                } else if (user_word.size() < 4) {
                    cout << "The entered word should be at least 4 letters long!" << endl;
                } else if (!english.contains(user_word)) {
                    cout << "The entered word is not an English word!" << endl;
                } else if (user_words.find(user_word) != user_words.cend()) {
                    cout << "You have entered this word before!" << endl;
                } else {
                    cout << "The entered word is not one of the answers!" << endl;
                }
                continue;
            } else {
                for (auto const &c : user_word) {
                    auto curr = boggle_chars.cbegin();
                    while ((curr = find(curr + 1, boggle_chars.cend(), c)) != boggle_chars.cend()) {
                        auto char_idx = curr - boggle_chars.cbegin();
                        highlightCube(char_idx/5, char_idx%5, true);
                    }
                }
                pause(100);
                break;
            }
        }
        if (quit) {
            break;
        }
        user_words.insert(user_word);
        words_ptr->erase(user_word);
        recordWordForPlayer(user_word, HUMAN);
    }

    // The computer's turn
    for (const auto &computer_word : (*words_ptr)) {
        recordWordForPlayer(computer_word, COMPUTER);
    }
}


// **************************************************************************************



/**
 * Function: playBoggle
 * --------------------
 * Manages all details needed for the user to play one
 * or more games of Boggle.
 */
static void playBoggle() {
    int dimension = getPreferredBoardSize();
    drawBoard(dimension, dimension);
    cout << "This is where you'd play the game of Boggle" << endl;

    // generate the boggle and all possible words based on the generated boggle
    auto boggle_ptr = draw_init_boggle(dimension);

    const string filename = "../assign-3-boggle/res/dictionary.txt";
    ifstream file_stream(filename);
    if (!file_stream.is_open()) {
        cout << "Dictionary file does not exist!" << endl;
    } else {
        file_stream.close();
    }
    auto english = Lexicon(filename);

    auto words_ptr = main_generate_all_words(boggle_ptr, english);

    // let the user enter the words
    start_game(boggle_ptr, words_ptr, english);

    // free the allocated resources
    delete words_ptr;
    delete boggle_ptr;
}

/**
 * Function: main
 * --------------
 * Serves as the entry point to the entire program.
 */
int main() {
    GWindow gw(kBoggleWindowWidth, kBoggleWindowHeight);
    initGBoggle(gw);
    welcome();
    if (getYesOrNo("Do you need instructions?")) giveInstructions();
    do {
        playBoggle();
    } while (getYesOrNo("Would you like to play again?"));
    cout << "Thank you for playing!" << endl;
    shutdownGBoggle();
    return 0;
}
