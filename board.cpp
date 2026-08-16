//
// Created by Striker-909 on 8/13/26.
//

#include "board.h"

#include <complex>
#include <set>
#include <functional>
#include <vector>
#include <iostream>

using namespace std;

const BoardIdentifier BoardIdentifier::ZERO_BOARD = BoardIdentifier(vector({vector({0})}));

size_t BoardIdentifier::hash_board(const std::vector<std::vector<int> > &board) {
    size_t hash = 0;
    bool complete = true;
    for (const auto & row : board) {
        for (int n : row) {
            if (complete && n == 0) { complete = false; }
            hash = (hash << 5) - hash + 37 * n;
            hash |= 0;
        }
    }
    hash = (hash << 1) ^ complete;
    return hash;
}

BoardIdentifier::BoardIdentifier(const std::vector<std::vector<int> > &board) :
    board(board), board_size(board.size()), box_size(static_cast<unsigned long>(sqrt(board_size))),
    hash(hash_board(board)), complete((hash & 1) == 1) {}

bool BoardIdentifier::operator==(const BoardIdentifier &other) const {
    if (this->hash != other.hash) { return false; }
    return this->board == other.board;
}

bool BoardIdentifier::operator<(const BoardIdentifier &other) const {
    return this->hash < other.hash;
}

string BoardIdentifier::to_string() const {
    stringstream out;
    for (int i = 0; i < board_size; i++) {
        auto const &row = board[i];
        for (int j = 0; j < row.size(); j++) {
            out << row[j] << "  " << (j % box_size == box_size - 1 ? "  " : "");
        }
        out << "\n" << (i % box_size == box_size - 1 ? "\n" : "");
    }
    return out.str().substr(0, out.str().size() - 1);
}

int Square::depth = 0;
string Square::telemetry_string{};

Square::Square(const int &value, const int x, const int y) : x(x), y(y) {
    possibilities.insert(value);
    saved_value = value;
    this->neighborhood = []() -> std::set<int> { return {}; };
}

Square::Square(const std::vector<int> &possibilities, const std::function<std::set<int>()> &neighborhood_fetcher, const int x, const int y) : x(x), y(y) {
    for (int i: possibilities) { this->possibilities.insert(i); }
    this->neighborhood = neighborhood_fetcher;
}

std::set<int> Square::get_possibilities() {
    return possibilities;
}

void Square::update(int max_update_depth, bool telemetry) {
    if (this->updating || depth >= max_update_depth) { return; }
    depth += 1;
    string telemetry_save;
    if (telemetry) {
        cout << telemetry_string << "┌ Updating (" << x << ", " << y << ")" << endl;
        telemetry_save = telemetry_string;
        telemetry_string += "│";
    }
    this->updating = true;
    auto neighbors = this->neighborhood();
    for (int neighbor : neighbors) { this->remove(neighbor); }
    if (telemetry) {
        telemetry_string = telemetry_save;
        cout << telemetry_string << "└ Finished Updating (" << x << ", " << y << ")" << endl;
    }
    depth -= 1;
    this->updating = false;
}

int Square::value(int max_update_depth) {
    if (possibilities.empty()) { return -1; }
    if (updating) { return saved_value; }
    this->update(max_update_depth);
    if (possibilities.size() == 1) { for (int n : possibilities) { saved_value = n; return n; } }
    return 0;
}

bool Square::remove(int value) {
    if (!possibilities.contains(value)) { return false; }
    possibilities.erase(value);
    return true;
}

void Square::set(int value) {
    possibilities.clear();
    possibilities.insert(value);
}

SudokuBoard::SudokuBoard(const int &max_value, const int &box_size, const std::vector<std::vector<int> > &raw_board, const std::vector<std::vector<Square>> &possibility_board) :
    max_value(max_value), box_size(box_size),
    raw_board(raw_board), possibility_board(possibility_board) {}

SudokuBoard::SudokuBoard(const vector<vector<int>>& board) {
    max_value = static_cast<int>(board.size());
    box_size = static_cast<int>(sqrt(max_value));
    this->raw_board = board;
    this->possibility_board = vector<vector<Square>>();
    this->generate_possibility_board();
}

void SudokuBoard::generate_possibility_board() {
    this->possibility_board.clear();
        vector<int> possible_values;
        for (int i = 1; i <= max_value; i++) { possible_values.push_back(i); }

        function<set<int>(int x, int y)> rows[max_value];
        function<set<int>(int x, int y)> columns[max_value];
        function<set<int>(int x, int y)> boxes[max_value];

        for (int i = 0; i < max_value; i++) {
            rows[i] = [i, this](int x, int y) -> set<int> {
                set<int> neighborhood = {};
                for (int j = 0; j < max_value; j++) {
                    if (j == x && i == y) { continue; }
                    neighborhood.insert(this->possibility_board[i][j].value());
                }
                 return neighborhood;
            };

            columns[i] = [i, this](int x, int y) -> set<int> {
                set<int> neighborhood = {};
                for (int j = 0; j < max_value; j++) {
                    if (i == x && j == y) { continue; }
                    neighborhood.insert(this->possibility_board[j][i].value());
                }
                return neighborhood;
            };

            int box_x = i % box_size * box_size;
            int box_y = i / box_size * box_size;
            boxes[i] = [box_x, box_y, this](int x, int y) -> set<int> {
                set<int> neighborhood = {};
                for (int y_ = box_y; y_ < box_y + this->box_size; y_++) {
                    for (int x_ = box_x; x_ < box_x + this->box_size; x_++) {
                        if (x_ == x || y_ == y) { continue; }
                        neighborhood.insert(this->possibility_board[y_][x_].value());
                    }
                }
                return neighborhood;
            };
        }

        for (int y = 0; y < max_value; y++) {
            this->possibility_board.emplace_back();
            for (int x = 0; x < max_value; x++) {

                int n = raw_board[y][x];
                if (n != 0) {
                    Square square = Square(raw_board[y][x], x, y);
                    this->possibility_board[y].emplace_back(square);
                    continue;
                }

                auto row_neighborhood = rows[y];
                auto column_neighborhood = columns[x];
                int box_i = (y / 3 * 3) + (x / 3);
                auto box_neighborhood = boxes[box_i];

                auto neighborhood = [row_neighborhood, column_neighborhood, box_neighborhood, x, y]() -> set<int> {
                    set<int> neighbors = {};
                    neighbors.merge(row_neighborhood(x, y));
                    neighbors.merge(column_neighborhood(x, y));
                    neighbors.merge(box_neighborhood(x, y));
                    return neighbors;
                };

                Square square = Square(possible_values, neighborhood, x, y);
                this->possibility_board[y].emplace_back(square);
            }
        }
}

void SudokuBoard::update_board() {
    for (const auto& row : possibility_board) {
        for (auto square : row) {
            square.update();
        }
    }
}

std::pair<std::pair<int, int>, std::set<int>> SudokuBoard::find_minimum_possibilities() {
    int num = max_value; int x = 0; int y = 0;
    for (int i = 0; i < max_value; i++) {
        for (int j = 0; j < max_value; j++) {
            auto n = possibility_board[i][j].get_possibilities().size();
            if (n > 1 && n < num) {
                num = n;
                x = j; y = i;
                if (n == 2) { break; }
            }
        }
    }
    return {{x, y}, possibility_board[y][x].get_possibilities()};
}

vector<vector<int>> SudokuBoard::extract_board_from_possibilities() {
    vector<vector<int>> board;
    for (const auto& row : possibility_board) {
        board.emplace_back();
        for (auto square : row) {
            auto value = square.value();
            if (value == -1) { return vector({vector{0}}); }
            board.back().emplace_back(value);
        }
    }
    return board;
}

BoardIdentifier SudokuBoard::generate_board_identifier() {
    return BoardIdentifier(this->extract_board_from_possibilities());
}

SudokuBoard SudokuBoard::deep_copy() {
    return SudokuBoard(this->max_value, this->box_size, this->raw_board, this->possibility_board);
}

void SudokuBoard::set_square(const int x, const int y, const int value) {
    this->possibility_board[y][x].set(value);
    this->raw_board[y][x] = value;
}

void SudokuBoard::remove_possibility(const int x, const int y, const int value) {
    this->possibility_board[y][x].remove(value);
}