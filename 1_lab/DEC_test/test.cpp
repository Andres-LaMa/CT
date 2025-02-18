#include <iostream>

int main(){
    int a = 0;
    for (int i = 0; i < 6; i++){
        a += 2;
        std::cout << a << " " << i << "\n";
    }
    int a1 = 123;
    int b = 456;
    int a2 = 7890;
    
    int a3 = 123.456;
    int a4 = 0x123;
    int a5 = 0b101;
    return 0;
}