#include <cmath>
void quadratic(int a, int b, int c, double &root1, double &root2){
    root1 = (-b + sqrt(pow(b, 2) - 4*a*c)) / (2*a);
    root2 = (-b - sqrt(pow(b, 2) - 4*a*c)) / (2*a);
}
