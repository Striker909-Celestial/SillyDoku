//
// Created by Striker-909 on 8/13/26.
//

#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include <functional>
#include <set>
#include <vector>

struct BoardIdentifier {
    static const BoardIdentifier ZERO_BOARD;

    const std::vector<std::vector<int>> board;
    const unsigned long board_size;
    const unsigned long box_size;

    const std::size_t hash;
    const bool complete = false;

    /// Hashes a 2D vector representing a sudoku board.
    /// @param board A 2D vector representing a sudoku board.
    /// @return A unique hash code for that board.
    static size_t hash_board(
        const std::vector<std::vector<int>>& board);

    /// A token holding a 2D vector of ints representing a sudoku board state.
    /// Also contains a hash code for the board.
    /// @param board A 2D vector representing a sudoku board.
    explicit BoardIdentifier(
        const std::vector<std::vector<int>>& board);

    [[nodiscard]] std::string to_string() const;

    bool operator==(const BoardIdentifier & other) const;

    bool operator<(const BoardIdentifier & other) const;
};

template<>
struct std::hash<BoardIdentifier> {
    std::size_t operator()(const BoardIdentifier& id) const noexcept {
        return id.hash;
    }
};

/// Represents an individual square in a sudoku board with a number of possible values.
class Square {
    static constexpr int MAX_UPDATE_DEPTH = 3;
    static std::string telemetry_string;
    static int depth;

    const int x; const int y;
    std::set<int> possibilities;
    std::function<std::set<int>()> neighborhood;
    bool updating = false;
    int saved_value = 0;

public:
    /// Represents an individual square in a sudoku board with a given value.
    /// @param value The value of this square.
    /// @param x The column this square occupies, with the left-most column being 0.
    /// @param y The row this square occupies, with the top-most row being 0.
    explicit Square(const int & value, int x, int y);
    /// Represents an individual square in a sudoku board with a number of possible values.
    /// @param possibilities A vector of all possible values this square could have.
    /// @param neighborhood_fetcher A supplier for a set of all values that are in the same row, column, or box as this square.
    /// @param x The column this square occupies, with the left-most column being 0.
    /// @param y The row this square occupies, with the top-most row being 0.
    explicit Square(const std::vector<int> & possibilities, const std::function<std::set<int>()> &neighborhood_fetcher, int x, int y);

    std::set<int> get_possibilities();

    /// Updates the possible values for this square based on the values present in the same row, column, or box as this square.
    ///
    /// Updating one square can trigger others to update, so a low max update depth is important to avoid exponential complexity.
    /// @param max_update_depth The maximum depth updates can reach before updates are canceled.
    /// @param telemetry If telemetry should be printed to the console as this square updates.
    void update(int max_update_depth = MAX_UPDATE_DEPTH, bool telemetry = false);
    int value(int max_update_depth = MAX_UPDATE_DEPTH);
    bool remove(int value);
    void set(int value);
};

class SudokuBoard {
    int max_value;
    int box_size;
    std::vector<std::vector<int>> raw_board;
    std::vector<std::vector<Square>> possibility_board;

    explicit SudokuBoard(const int & max_value, const int & box_size, const std::vector<std::vector<int>> & raw_board, const std::vector<std::vector<Square>> & possibility_board);

    void generate_possibility_board();

public:
    explicit SudokuBoard(
        const std::vector<std::vector<int>>& board);

    void update_board();

    std::vector<std::vector<int>>
        extract_board_from_possibilities();

    std::pair<std::pair<int, int>, std::set<int>>
        find_minimum_possibilities();

    BoardIdentifier generate_board_identifier();

    SudokuBoard deep_copy();

    void set_square(int x, int y, int value);

    void remove_possibility(int x, int y, int value);
};


#endif //SUDOKU_BOARD_H
