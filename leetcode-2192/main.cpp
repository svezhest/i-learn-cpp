#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <unordered_set>
#include <vector>

class Solution {
  public:
    std::vector<std::vector<int>>
    getAncestors(int n, std::vector<std::vector<int>> &edges) {
        auto ancestors = std::vector<std::unordered_set<int>>(n);

        auto graph = std::vector<std::vector<int>>(n);
        auto reverse_graph = std::vector<std::vector<int>>(n);
        for (auto p : edges) {
            graph[p[0]].push_back(p[1]);
            reverse_graph[p[1]].push_back(p[0]);
        }

        std::map<int, std::unordered_set<int>> vertices_by_number_of_ancestors;

        auto insert_to_set = [&vertices_by_number_of_ancestors](int idx,
                                                                int vertex) {
            (vertices_by_number_of_ancestors.contains(idx)
                 ? vertices_by_number_of_ancestors.at(idx)
                 : vertices_by_number_of_ancestors
                       .insert(std::make_pair(idx, std::unordered_set<int>()))
                       .first->second)
                .insert(vertex);
        };

        for (int vertex = 0; vertex < n; ++vertex) {
            insert_to_set(reverse_graph[vertex].size(), vertex);
        }

        auto current_locations = std::vector<int>(n);
        for (auto &[location, set] : vertices_by_number_of_ancestors) {
            for (int vertex : set) {
                current_locations[vertex] = location;
            }
        }

        auto remove_ansestor_of_vertex = [&](int vertex, int ancestor) {
            int current_location = current_locations[vertex]--;
            vertices_by_number_of_ancestors[current_location--].erase(vertex);
            vertices_by_number_of_ancestors[current_location].insert(vertex);
        };

        while (vertices_by_number_of_ancestors.size() > 0) {
            auto &nodes = vertices_by_number_of_ancestors.begin()->second;
            if (nodes.size() == 0) {
                vertices_by_number_of_ancestors.erase(
                    vertices_by_number_of_ancestors.begin());
                continue;
            }

            int node = *nodes.begin();
            nodes.erase(node);

            for (auto prev : reverse_graph[node]) {
                for (auto x : ancestors[prev]) {
                    ancestors[node].insert(x);
                }
                ancestors[node].insert(prev);
            }
            for (auto next : graph[node]) {
                remove_ansestor_of_vertex(next, node);
            }
        }

        std::vector<std::vector<int>> answer;
        for (auto &v : ancestors) {
            auto temp = std::vector<int>(v.begin(), v.end());
            sort(temp.begin(), temp.end());
            answer.push_back(temp);
        }
        return answer;
    }
};

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

int main() {
    std::vector<std::vector<int>> test_case = {
        {3, 6}, {2, 4}, {8, 6}, {7, 4}, {1, 4}, {2, 1}, {7, 2},
        {0, 4}, {5, 0}, {4, 6}, {3, 2}, {5, 6}, {1, 6}};
    std::cout << Solution().getAncestors(9, test_case) << '\n';
    return 0;
}
