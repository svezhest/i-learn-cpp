#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

class Solution {
    using answer = vector<vector<int>>;
    unordered_map<int, answer> m;

    string get_hash(vector<int> &vec) {
        std::stringstream ss;
        for (auto x : vec) {
            ss << x << ',';
        }
        return ss.str();
    }

    template <typename T>
    typename std::vector<T>::iterator insert_sorted(std::vector<T> &vec,
                                                    T const &item) {
        return vec.insert(std::upper_bound(vec.begin(), vec.end(), item), item);
    }

  public:
    answer combinationSum(vector<int> &candidates, int target) {
        if (target < 0) {
            return answer{};
        }
            

        if (target == 0) {
            return answer{{}};
        }

        if (m.contains(target)) {
            return m.at(target);
        }

        answer ans;
        unordered_set<string> hashes;

        for (auto x : candidates) {
            answer temp = combinationSum(candidates, target - x);
            for (auto &t : temp) {
                insert_sorted(t, x);
                string temp_hash = get_hash(t);
                if (!hashes.contains(temp_hash)) {
                    ans.push_back(t);
                    hashes.insert(temp_hash);
                }
            }
        }

        return ans;
    }
};

ostream &operator<<(ostream &os, const vector<int> &v) {
    for (auto x : v) {
        os << x << ", ";
    }
    return os;
}

ostream &operator<<(ostream &os, const vector<vector<int>> &v) {
    for (const auto &vv : v) {
        os << '(' << vv << "), ";
    }
    return os;
}

int main() {
    vector<int> a = {2};
    cout << Solution().combinationSum(a, 1);
}
