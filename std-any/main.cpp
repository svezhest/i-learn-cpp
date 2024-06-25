#include <any>
#include <iostream>
#include <vector>

int main() {
    std::vector<std::any> v;
    v.push_back(1);
    v.push_back(3.14);

    for (const auto x : v) {
        std::cout << (std::string(x.type().name()).compare("i") == 0
                          ? std::any_cast<int>(x)
                          : std::any_cast<double>(x))
                  << ' ';
    }

    std::cout << '\n';

    std::any a;

    a = 2;
    a = 3.14;
    a = "str";

    std::cout << std::any_cast<const char *>(a) << '\n';
}
