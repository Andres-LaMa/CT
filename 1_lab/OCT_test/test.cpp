#include <iostream>

int main(){
    int a = 0;
    for (int i = 0; i < 6; i++){
        a += 2;
        std::cout << a << " " << i << "\n";
    }
    int a1 = 012;
    int b = 0345;
    int a2 = 077;
    
    int a3 = 89;
    int a4 = 0128;
    int a5 = 0x123;
    return 0;
}