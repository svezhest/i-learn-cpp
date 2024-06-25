#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

class Solution {
  public:
    int minKBitFlips(std::vector<int> &nums, int k) {
        if (k <= 1) {
            int ones_to_fill =
                nums.size() - std::accumulate(nums.begin(), nums.end(), 0);
            return k == 1 ? ones_to_fill : ones_to_fill == 0 ? 0 : -1;
        }

        int res = 0;
        bool is_flipped = false;
        std::queue<int> queue;

        for (size_t idx = 0; idx < nums.size(); ++idx) {
            bool current = static_cast<bool>(nums[idx]);

            if (queue.size() > 0 && queue.front() == idx) {
                is_flipped = !is_flipped;
                queue.pop();
            }

            current ^= is_flipped;

            if (!current) {
                is_flipped = !is_flipped;
                queue.emplace(idx + k);
                res += 1;
            }
        }

        if (queue.size() > 0 && queue.front() == nums.size()) {
            queue.pop();
        }

        return queue.size() == 0 ? res : -1;
    }
};

int main() {
    std::vector<int> a = {0, 0, 0, 1, 0, 1, 1, 0};
    std::cout << Solution().minKBitFlips(a, 3);
    return 0;
}
