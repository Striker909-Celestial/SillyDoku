//
// Created by Striker-909 on 8/13/26.
//

#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H
#include <unordered_map>

#include "board.h"

class Solver {
    std::unordered_map<BoardIdentifier, SudokuBoard*> board_database;

public:
    explicit Solver();

    BoardIdentifier solve(const BoardIdentifier & board);
    SudokuBoard* solve(SudokuBoard *board);
};


#endif //SUDOKU_SOLVER_H
