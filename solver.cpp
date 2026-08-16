//
// Created by Striker-909 on 8/13/26.
//

#include "solver.h"
#include <iostream>
using namespace std;

#include "board.h"

Solver::Solver() {
    this->board_database = unordered_map<BoardIdentifier, SudokuBoard*>();
    this->board_database[BoardIdentifier::ZERO_BOARD] = nullptr;
}

BoardIdentifier Solver::solve(const BoardIdentifier &board) {
    SudokuBoard sudoku_board = SudokuBoard(board.board);
    auto* result = solve(&sudoku_board);
    return result
        ? result->generate_board_identifier()
        : BoardIdentifier::ZERO_BOARD;
}

SudokuBoard* Solver::solve(SudokuBoard *board) {
    auto id = board->generate_board_identifier();
    if (this->board_database.contains(id)) { return this->board_database[id]; }

    if (id.complete) {
        this->board_database[id] = board;
        return board;
    }

    if (id.hash == 0) {
        this->board_database[id] = nullptr;
        return nullptr;
    }

    auto fork_target = board->find_minimum_possibilities();
    int x = fork_target.first.first; int y = fork_target.first.second;
    auto possibilities = fork_target.second;

    for (auto possibility : possibilities) {
        auto copy = new SudokuBoard(id.board);
        copy->set_square(x, y, possibility);
        auto out = this->solve(copy);
        if (out != nullptr) {
            this->board_database[id] = this->board_database[copy->generate_board_identifier()];
            return this->board_database[id];
        }
        board->remove_possibility(x, y, possibility);
    }

    this->board_database[id] = nullptr;
    return nullptr;
}