#include <iostream>
#include <string>

void nameDiamond(const std::string &str){
    for (std::string::size_type i = 1; i <= str.size(); ++i){
        std::cout << str.substr(0, i) << std::endl;
    }
    for (std::string::size_type i = str.size()-1; i >= 1; --i){
        std::cout << std::string(str.size()-i, ' ');
        std::cout << str.substr(str.size()-i) << std::endl;
    }
}
