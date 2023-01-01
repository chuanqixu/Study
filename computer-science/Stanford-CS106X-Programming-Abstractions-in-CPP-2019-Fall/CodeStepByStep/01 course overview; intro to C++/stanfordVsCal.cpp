#include <iostream>

int main() {
    int score_stanford, score_cal;
    std::cout << "Stanford: How many points did they score? ";
    std::cin >> score_stanford;
    std::cout << "Cal: How many points did they score? ";
    std::cin >> score_cal;
    if (score_stanford < score_cal)
        std::cout << "Cal won!" << std::endl;
    else if (score_stanford > score_cal)
        std::cout << "Stanford won!" << std::endl;
    else
        std::cout << "Draw!" << std::endl;
}