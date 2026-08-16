#include "board.h"

#include <iostream>
#include <vector>

#include "solver.h"

using namespace std;

int main() {

    const vector<vector<int>> easy_board = vector({
        vector({0, 0, 0, 1, 0, 0, 2, 0, 4}),
        vector({0, 0, 7, 5, 8, 6, 9, 1, 3}),
        vector({0, 1, 0, 3, 0, 0, 0, 0, 7}),
        vector({0, 3, 0, 6, 5, 0, 4, 7, 9}),
        vector({0, 0, 8, 7, 0, 3, 0, 6, 2}),
        vector({0, 0, 0, 2, 0, 0, 5, 0, 8}),
        vector({8, 7, 0, 0, 0, 0, 0, 0, 0}),
        vector({0, 4, 9, 0, 1, 0, 0, 2, 0}),
        vector({2, 5, 0, 4, 0, 0, 8, 9, 0})
    });

    const vector<vector<int>> medium_board = vector({
        vector({2, 0, 3, 4, 0, 0, 0, 0, 5}),
        vector({8, 0, 9, 1, 6, 0, 7, 0, 4}),
        vector({0, 0, 6, 0, 3, 0, 0, 1, 9}),
        vector({7, 0, 2, 0, 0, 3, 0, 6, 0}),
        vector({0, 0, 8, 2, 5, 0, 0, 0, 0}),
        vector({0, 0, 1, 6, 0, 7, 0, 0, 2}),
        vector({0, 0, 7, 0, 0, 5, 9, 2, 6}),
        vector({9, 3, 0, 7, 2, 0, 0, 0, 0}),
        vector({6, 0, 0, 0, 9, 0, 4, 7, 0})
    });

    const vector<vector<int>> hard_board = vector({
        vector({1, 0, 0, 0, 3, 4, 0, 0, 8}),
        vector({0, 7, 0, 6, 8, 0, 0, 3, 0}),
        vector({0, 0, 8, 2, 1, 0, 7, 0, 4}),
        vector({0, 5, 4, 0, 9, 0, 6, 8, 0}),
        vector({9, 1, 0, 5, 0, 8, 0, 2, 0}),
        vector({0, 8, 0, 3, 0, 0, 0, 0, 5}),
        vector({3, 0, 5, 9, 0, 6, 8, 7, 1}),
        vector({0, 0, 6, 0, 0, 0, 0, 4, 0}),
        vector({0, 0, 1, 0, 7, 0, 2, 0, 0})
    });

    const vector<vector<int>> expert_board = vector({
        vector({1, 5, 0, 0, 8, 2, 0, 0, 0}),
        vector({3, 0, 0, 0, 7, 0, 0, 1, 0}),
        vector({0, 0, 0, 0, 0, 0, 7, 5, 3}),
        vector({0, 0, 0, 5, 2, 7, 6, 0, 9}),
        vector({0, 0, 0, 0, 0, 0, 5, 0, 0}),
        vector({0, 4, 0, 0, 6, 3, 8, 0, 7}),
        vector({4, 0, 0, 0, 0, 8, 0, 0, 0}),
        vector({7, 0, 3, 0, 4, 0, 1, 0, 0}),
        vector({0, 0, 8, 6, 0, 0, 3, 0, 0})
    });

    const vector<vector<int>> extreme_board = vector({
        vector({3, 0, 0, 0, 4, 9, 0, 0, 0}),
        vector({0, 0, 0, 6, 0, 0, 5, 0, 1}),
        vector({7, 5, 2, 0, 0, 1, 0, 0, 0}),
        vector({0, 0, 1, 0, 0, 0, 7, 0, 0}),
        vector({5, 0, 0, 3, 9, 6, 0, 0, 0}),
        vector({0, 0, 8, 1, 5, 0, 0, 9, 6}),
        vector({0, 0, 3, 0, 1, 0, 0, 6, 0}),
        vector({0, 0, 4, 0, 1, 0, 0, 6, 0}),
        vector({0, 0, 0, 0, 2, 8, 0, 0, 0})
    });

    const vector<vector<int>> master_board = vector({
        vector({0, 0, 9, 5, 8, 6, 0, 0, 0}),
        vector({0, 0, 0, 0, 2, 0, 0, 0, 0}),
        vector({4, 0, 0, 0, 0, 0, 6, 8, 3}),
        vector({9, 0, 0, 6, 5, 0, 0, 3, 2}),
        vector({0, 6, 0, 7, 0, 0, 0, 9, 8}),
        vector({0, 3, 0, 2, 0, 0, 7, 0, 4}),
        vector({0, 0, 3, 0, 0, 0, 0, 0, 0}),
        vector({6, 2, 0, 0, 1, 5, 0, 4, 0}),
        vector({0, 0, 0, 4, 0, 0, 0, 5, 0})
    });

    const vector<vector<int>> nyt_hard_board = vector({
        vector({0, 0, 0, 0, 0, 0, 0, 0, 6}),
        vector({0, 8, 1, 0, 0, 5, 0, 0, 9}),
        vector({0, 0, 0, 0, 0, 7, 0, 8, 0}),
        vector({9, 0, 8, 0, 1, 6, 2, 0, 0}),
        vector({0, 2, 0, 4, 8, 0, 0, 0, 0}),
        vector({0, 0, 0, 0, 0, 0, 0, 0, 0}),
        vector({7, 9, 0, 6, 0, 0, 0, 3, 1}),
        vector({0, 0, 5, 0, 3, 0, 0, 0, 0}),
        vector({1, 0, 2, 0, 0, 9, 0, 0, 0})
    });

    const vector<vector<int>> impossible_board = vector({
        vector({1, 5, 0, 0, 8, 2, 0, 0, 0}),
        vector({3, 0, 2, 4, 7, 0, 0, 1, 0}),
        vector({8, 7, 9, 0, 0, 0, 7, 5, 3}),
        vector({0, 0, 0, 5, 2, 7, 6, 0, 9}),
        vector({0, 0, 0, 0, 0, 0, 5, 0, 0}),
        vector({0, 4, 0, 0, 6, 3, 8, 0, 7}),
        vector({4, 0, 0, 0, 0, 8, 0, 0, 0}),
        vector({7, 6, 3, 0, 4, 0, 1, 0, 0}),
        vector({0, 0, 8, 6, 0, 0, 3, 0, 0})
    });

    const vector<vector<int>> blank_board = vector({
        vector({1, 0, 0, 0, 0, 0, 0, 0, 0}),
        vector({0, 0, 0, 0, 0, 0, 0, 0, 0}),
        vector({0, 0, 0, 0, 0, 0, 0, 0, 0}),
        vector({0, 0, 0, 0, 0, 0, 0, 0, 0}),
        vector({0, 0, 0, 0, 0, 0, 0, 0, 0}),
        vector({0, 0, 0, 0, 0, 0, 0, 0, 0}),
        vector({0, 0, 0, 0, 0, 0, 0, 0, 0}),
        vector({0, 0, 0, 0, 0, 0, 0, 0, 0}),
        vector({0, 0, 0, 0, 0, 0, 0, 0, 0})
    });

    BoardIdentifier board = BoardIdentifier(blank_board);

    Solver solver = Solver();
    BoardIdentifier board_out = solver.solve(board);

    cout << endl << board_out.to_string() << endl;

    cout << (board_out.complete ? "complete" : "incomplete") << endl;

    return 0;
}
