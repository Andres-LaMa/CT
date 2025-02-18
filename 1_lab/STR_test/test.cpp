#include <iostream>

int main(){
    int a = 0;
    for (int i = 0; i < 6; i++){
        a += 2;
        std::cout << a << " " << i << "\n";
    }
    char* str1 = "abc";
    char* str2 = "123";
    char* str3 = "a\"bc";
    char* str4 = "a\\bc";

    char* str5 = "abc;
    char* str6 = abc";
    char* str7 = "a\bc";
    
    return 0;
}