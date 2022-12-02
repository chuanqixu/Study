#include <iostream>
#include <cmath>

int bmi_class(double bmi){
    if (bmi < 18.5)
        return 1;
    else if (bmi < 24.9)
        return 2;
    else if (bmi < 29.9)
        return 3;
    else
        return 4;
}

int main() {
    double bmi1, bmi2, height, weight;

    std::cout << "This program reads data for two people\n"
                "and computes their body mass index (BMI).\n\n"
                "Enter Person 1's information:\n"
                "height (in inches)? ";
    std::cin >> height;
    std::cout << "weight (in pounds)? ";
    std::cin >> weight;
    
    bmi1 = weight/pow(height, 2) * 703;
    std::cout << "BMI = " << bmi1 << ", class " << bmi_class(bmi1) << std::endl << std::endl;

    std::cout << "Enter Person 2's information:\n"
                "height (in inches)? ";
    std::cin >> height;
    std::cout << "weight (in pounds)? ";
    std::cin >> weight;
    
    bmi2 = weight/pow(height, 2) * 703;
    std::cout << "BMI = " << bmi2 << ", class " << bmi_class(bmi2) << std::endl << std::endl;

    std::cout << "BMI difference = " << abs(bmi1 - bmi2) << std::endl;

    return 0;
}
