#pragma GCC optimize("Ofast")
#pragma GCC target("avx2,tune=native")

#include <iostream>
#include <vector>

bool init() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    return true;
}

bool _ = init();

class Solution {
  public:
    std::vector<int> maxSlidingWindow(std::vector<int> &nums, const int k) {
        short q[nums.size()];
        unsigned l = 0, r = 0;
        unsigned idx = 0;
        unsigned k_idx = 0;
        while (idx < k - 1) {
            while (l < r && q[r - 1] < nums[idx]) {
                --r;
            }
            q[r++] = nums[idx++];
        }

        while (idx < nums.size()) {
            while (l < r && q[r - 1] < nums[idx]) {
                --r;
            }
            q[r++] = nums[idx];

            if (q[l] == nums[k_idx]) {
                ++l;
            } else {
                nums[k_idx] = q[l];
            }

            ++idx;
            ++k_idx;
        }

        nums.resize(k_idx);

        return std::move(nums);
    }
};
