cmake_minimum_required(VERSION 4.2)
project(Sudoku)

set(CMAKE_CXX_STANDARD 26)

add_executable(Sudoku main.cpp
        board.cpp
        board.h
        solver.cpp
        solver.h)
