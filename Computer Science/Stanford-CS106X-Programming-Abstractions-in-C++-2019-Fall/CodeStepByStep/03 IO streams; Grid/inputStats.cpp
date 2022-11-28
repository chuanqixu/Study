#include <iostream>
#include <fstream>
#include <string>

void inputStats(std::string filename){
    std::ifstream file(filename);
    if (file.is_open()){
        std::string line;
        long long line_no = 1;
        long long num_chars = 0;
        std::string::size_type max_length = 0;
        while(getline(file, line)){
            std::cout << "Line " << line_no 
                    << " has " << line.size() 
                    << " chars" << std::endl;
            ++line_no;
            num_chars += line.size();
            if (max_length < line.size()){
                max_length = line.size();
            }
        }
        --line_no;
        std::cout << line_no << " lines; longest = " << max_length 
                << ", average = " << static_cast<double>(num_chars)/line_no 
                << std::endl;
    }
}

int main(){
    inputStats("carroll.txt");
    return 0;
}