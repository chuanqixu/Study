#include <cmath>
#include <string>

class Circle {
public:
    Circle(double r): radius(r) {}
    double area() { return M_PI * radius * radius;}
    double circumference() { return 2 * M_PI * radius;}
    double getRadius() { return radius; }
    std::string toString() { return std::string("Circle{radius=") + std::to_string(radius) + "}"; }

private:
    double radius;
};