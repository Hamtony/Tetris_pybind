cmake_minimum_required(VERSION 3.15)

project(TetrisGame VERSION 0.1.0)

add_subdirectory(pybind11)
set (PYBIND11_PYTHON_VERSION "3.13.1")
pybind11_add_module(cppTetris main.cpp)