#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

class Solution {
  public:
    int minFlips(std::vector<std::vector<int>> &mat) {
        auto flip = [&mat](const int i, const int j) {
            for (auto [ii, jj] :
                 {std::make_pair(i, j), std::make_pair(i - 1, j),
                  std::make_pair(i + 1, j), std::make_pair(i, j - 1),
                  std::make_pair(i, j + 1)}) {
                if (!(ii >= 0 && ii < mat.size() && jj >= 0 &&
                      jj < mat[0].size())) {
                    continue;
                }
                mat[ii][jj] = !mat[ii][jj];
            }
        };

        auto hash = [&mat]() {
            int res = 0;
            int n = 0;
            for (int i = 0; i < mat.size(); ++i) {
                for (int j = 0; j < mat[0].size(); ++j) {
                    res += ((1 << (n++)) * mat[i][j]);
                }
            }
            return res;
        };

        std::unordered_map<int, int> hashes;

        std::function<int()> dfs;
        dfs = [&]() {
            if (hashes.find(hash()) != hashes.end()) {
                return hashes[hash()];
            }
            bool solved = true;
            for (int i = 0; i < mat.size(); ++i) {
                for (int j = 0; j < mat[0].size(); ++j) {
                    if (mat[i][j]) {
                        solved = false;
                        break;
                    }
                }
                if (!solved) {
                    break;
                }
            }
            if (solved) {
                hashes[hash()] = 0;
                return 0;
            }
            hashes[hash()] = -1;
            for (int i = 0; i < mat.size(); ++i) {
                for (int j = 0; j < mat[0].size(); ++j) {
                    flip(i, j);
                    int res = dfs();
                    flip(i, j);
                    if (res != -1) {
                        ++res;
                        if (hashes[hash()] == -1 || res < hashes[hash()]) {
                            hashes[hash()] = res;
                        }
                    }
                }
            }
            return hashes[hash()];
        };

        return dfs();
    }
};

int main() {
    std::vector<std::vector<int>> a = {{0, 1}, {0, 0}};
    std::cout << Solution().minFlips(a);
    return 0;
}
