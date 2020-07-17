#include <iostream>
#include <set>


int main() {
    std::set <std::string> our_set;

    std::string str;
    while (std::getline(std::cin, str)) {
        our_set.insert(str);
    }

    std::cout << std::endl;
    for (const auto& elem : our_set) {
        std::cout << elem << std::endl;
    }
}
