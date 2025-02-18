#include <iostream>

int main(){
    int a = 0;
    for (int i = 0; i < 6; i++){
        a += 2;
        std::cout << a << " " << i << "\n";
    }
    int a1 = 0x123;
    int b = 0X456;
    int a2 = 0xABCDEF;
    
    int a3 = 0xGHIJ;
    int a4 = 0x123.456;
    int a5 = 0b101;
    return 0;
}