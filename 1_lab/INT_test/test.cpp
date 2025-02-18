#include <iostream>

int main(){
    int a = 0;
    for (int i = 0; i < 6; i++){
        a += 2;
        std::cout << a << " " << i << "\n";
    }
    int a1 = 123;
    int b = 123'456;
    int a2 = 123U;
    int a3 = 123LL;
    int a4 = 123'456'789Z;

    int a5 = 123.456;
    int a6 = 123,456;
    int a7 = 123';
    return 0;
}