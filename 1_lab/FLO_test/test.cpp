#include <iostream>

int main(){
    int a = 0;
    for (int i = 0; i < 6; i++){
        a += 2;
        std::cout << a << " " << i << "\n";
    }
    int a1 = 123.456;
    int b = 1.23e4;
    int a2 = 1.23E-4;
    int a3 = 1.23f;
    int a4 = 1.23L;

    int a5 = 123.;
    int a5 = .456;
    int a5 = 1.23e;
    int a5 = 1.23e+;
    return 0;
}