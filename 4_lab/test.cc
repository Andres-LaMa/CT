#include <iostream>
#include <vector>

int main() {
    typedef std::vector<int> *intList;
    auto list = intList(10, 0);

    for (const auto& num: list) {
        std::cout << num << " ";
    }
    std::cout << "\n";
    
    return 0;
}