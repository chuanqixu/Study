#include <iostream>

using namespace std;

int main() {
    int v1 = 10;
    int v2 = 25;
    int* p1 = &v1;
    int* p2 = &v2;
    
    *p1 += *p2;
    p2 = p1;
    *p2 = *p1 + *p2;
    
    cout << v1 << " " << v2 << endl; // 70 25
    cout << *p1 << " " << *p2 << endl; // 70 70
    

    return 0;
}
