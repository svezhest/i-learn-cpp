#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Solution {
  public:
    string multiply(string num1, string num2) {
        auto v1 = num1 | views::reverse;
        auto v2 = num2 | views::reverse;

        vector<int> res;

        auto pb = [](auto &v, int idx, int x) {
            if (idx < v.size()) {
                x += v[idx];
                v[idx] = x % 10;
            } else if (idx == v.size()) {
                v.push_back(x % 10);
            } else {
                throw runtime_error("fuck");
            }

            return x / 10;
            ;
        };

        int idx1 = 0;
        for (char ch1 : v1) {
            int idx2 = 0;
            int acc = 0;
            for (char ch2 : v2) {
                acc = pb(res, idx1 + idx2, (ch1 - '0') * (ch2 - '0') + acc);
                ++idx2;
            }
            while (acc) {
                acc = pb(res, idx1 + idx2++, acc);
            }
            ++idx1;
        }

        stringstream ss;
        bool zero = true;
        for (int x : res | views::reverse |
                         views::drop_while([](int x) { return x == 0; })) {
            ss << x;
            zero = false;
        }
        if (zero) {
            ss << 0;
        }
        return ss.str();
    }
};

int main() { cout << Solution().multiply("123", "456"); }