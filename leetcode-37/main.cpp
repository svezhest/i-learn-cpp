#include <cstdint>
#include <iostream>
#include <ranges>
#include <vector>

class Solution {
  public:
    void solveSudoku(std::vector<std::vector<char>> &board) {
        uint_fast16_t rows[9] = {0};
        uint_fast16_t cols[9] = {0};
        uint_fast16_t cells[9] = {0};

        auto find_num = [](uint_fast16_t possibilities) -> uint_fast16_t {
            for (uint_fast16_t num = 1; num <= 9; ++num) {
                if (possibilities == 1 << num) {
                    return num;
                }
            }
            return 0;
        };

        size_t to_fill = 0;

        for (size_t idx = 0; idx < 81; ++idx) {
            const size_t row = idx / 9;
            const size_t col = idx % 9;

            if (board[row][col] == '.') {
                ++to_fill;
                continue;
            }

            const size_t cell = (row / 3) * 3 + col / 3;

            const uint_fast16_t bit = 1 << (board[row][col] - '0');

            rows[row] |= bit;
            cols[col] |= bit;
            cells[cell] |= bit;
        }

        int max = 0;
        while (to_fill && ++max < 1000) {
            for (size_t idx = 0; idx < 81; ++idx) {
                const size_t row = idx / 9;
                const size_t col = idx % 9;

                if (board[row][col] != '.') {
                    continue;
                }

                const size_t cell = (row / 3) * 3 + col / 3;

                const uint_fast16_t possibilities =
                    (1 << 10) - 2 - rows[row] | cols[col] | cells[cell];

                if (const uint_fast16_t n = find_num(possibilities)) {
                    board[row][col] = '0' + n;
                    --to_fill;
                    rows[row] |= (1 << n);
                    cols[col] |= (1 << n);
                    cells[cell] |= (1 << n);
                    break;
                }
            }
        }
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
    // std::vector<std::vector<char>> board = {
    //     {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
    //     {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
    //     {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
    //     {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
    //     {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
    //     {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
    //     {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
    //     {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
    //     {'.', '.', '.', '.', '8', '.', '.', '7', '9'}};

    std::vector<std::vector<char>> board = {
        {'.', '.', '9', '7', '4', '8', '.', '.', '.'},
        {'7', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '2', '.', '1', '.', '9', '.', '.', '.'},
        {'.', '.', '7', '.', '.', '.', '2', '4', '.'},
        {'.', '6', '4', '.', '1', '.', '5', '9', '.'},
        {'.', '9', '8', '.', '.', '.', '3', '.', '.'},
        {'.', '.', '.', '8', '.', '3', '.', '2', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '6'},
        {'.', '.', '.', '2', '7', '5', '9', '.', '.'}};

    Solution().solveSudoku(board);
    std::cout << board;
}