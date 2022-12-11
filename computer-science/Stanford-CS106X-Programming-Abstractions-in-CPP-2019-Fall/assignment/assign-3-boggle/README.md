# Introduction

[Assignment description](./15-Assignment-3-Boggle.pdf).

This assignment implements a game named [Boggle](https://en.wikipedia.org/wiki/Boggle).

The main part of this assignment is to use recursion to find all possible English words that are longer than 4 letters in the boggle.

# Implementation

## Contruct the Letter Cubes

According to the handout, the letter cubes need to be randomized every time playing the game, which consists of two steps:

1. Randomize the predefined cubes by randomly shuffling the string representing the possible letters for cubes.
2. Randomly select a character from the string to be placed in the boggle on the specific location.

Also, need to implement a way to let the user specify the cubes manually.

This section is not hard, so I leave out the details.

## Precompute All Formable Words

In this section, given the boggle, need to find all possible English words that are longer than 4 letters.

The recursive parameters are:

1. A `string` representing the prefix of the word.
2. Coordinates of the character to be checked and added in the current recursive step.
3. A `set` containing whether a location has been previously visited.

The recursive steps are:

1. Find all in bound neighbor characters that are not visited before.
2. Add the character to the end of the prefix, and check if there are English words with the same prefix. 
    - If yes, then check whether this prefix is an English word with length larger than 3 letters, and add it to the set containing all words if yes. Then recursively do the checking starting from this letter.
    - If no, then nothing happens.

To run the code, the main entry function choose every location in the boggle and start checking from them.

The code is below:

```cpp
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
```


## Start the Game

Let the user enter the word and check if it satisfies the requirements. If it is, record this word and delete the word from the set containing all possible words.

After the user finishes, record all remaining words in the set.
