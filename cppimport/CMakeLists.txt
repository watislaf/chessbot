cmake_minimum_required(VERSION 3.4...3.18)
project(chess)

add_subdirectory(pybind11 REQUIRED)
pybind11_add_module(CHESSAI class.cpp)

