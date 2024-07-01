#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &p) {
    return os << '(' << p.first << ',' << p.second << ')';
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
    os << '[';
    for (const auto &x : vec | std::views::take(1)) {
        os << x;
    }
    for (const auto &x : vec | std::views::drop(1)) {
        os << ',' << x;
    }
    return os << ']';
}

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};

std::ostream &operator<<(std::ostream &os, TreeNode *node) {
    if (!node) {
        return os << "null";
    }
    return os << '(' << node->val << ';' << node->left << ',' << node->right
              << ')';
}

class Solution {
    TreeNode *reconstruct(size_t depth,
                          const std::vector<std::pair<size_t, int>> &vec,
                          size_t begin, size_t end) {
        if (begin == end) {
            return nullptr;
        }

        int left = begin + 1;
        int right = end;

        for (size_t idx = begin + 2; idx < end; ++idx) {
            if (vec[idx].first == depth + 1) {
                right = idx;
            }
        }

        return new TreeNode(vec.at(begin).second,
                            reconstruct(depth + 1, vec, left, right),
                            reconstruct(depth + 1, vec, right, end));
    }

  public:
    TreeNode *recoverFromPreorder(std::string traversal) {
        std::vector<std::pair<size_t, int>> parsed_traversal;

        size_t depth = 0;
        int num = 0;

        auto add = [&parsed_traversal, &num, &depth]() {
            if (num != 0) {
                parsed_traversal.emplace_back(depth, num);
                num = 0;
                depth = 0;
            }
        };

        for (char ch : traversal) {
            if (ch == '-') {
                add();
                ++depth;
            } else {
                num = (num * 10) + (ch - '0');
            }
        }
        add();

        return reconstruct(0, parsed_traversal, 0, parsed_traversal.size());
    }
};

int main() {
    std::cout << Solution().recoverFromPreorder("1-2--3--4-5--6--7");
}
