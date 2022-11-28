#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>


void hoursWorked(std::string filename){
    std::ifstream file(filename);
    if (file.is_open()){
        std::string line, id, name;
        double hour, total_hour;
        int num_days;
        while(getline(file, line)){
            total_hour = 0;
            num_days = 0;
            std::istringstream stream_line(line);
            stream_line >> id >> name;
            while(stream_line >> hour){
                total_hour += hour;
                ++num_days;
            }
            std::cout << std::setw(9) << std::left << name 
                    << "(ID# " << std::fixed << std::setw(4) 
                    << std::right << id
                    << ") worked " << std::setprecision(1) 
                    << total_hour << " hours ("
                    << std::setprecision(2)
                    << total_hour/num_days << "/day)" << std::endl;
        }
    }
}

int main(){
    hoursWorked("hours.txt");
    return 0;
}