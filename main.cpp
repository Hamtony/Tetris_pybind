#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <complex.h>

#include <vector>
#include "tetris.h"

using namespace std;

namespace py = pybind11;

PYBIND11_MODULE(cppTetris, handle){
    handle.doc() = "ayuda no duermo";
    
    
    py::class_<Tetromino>(handle, "Tetromino")
    .def(py::init<>())
    .def("getType", &Tetromino::getType)
    .def("getRotation", &Tetromino::getRotation)
    .def("getX", &Tetromino::getX)
    .def("getY", &Tetromino::getY)
    .def("getColor", &Tetromino::getColor)
    .def("getImage", &Tetromino::getImage)
    ;
    py::class_<Tetris>(handle, "Tetris")
    .def(py::init<>())
    .def("getBoard", &Tetris::getBoard)
    .def("getCurrent", &Tetris::getCurrent)
    .def("hold_piece", &Tetris::hold_piece)
    .def("rotate", &Tetris::rotate)
    .def("move", &Tetris::move)
    .def("drop", &Tetris::drop)
    .def("soft_drop", &Tetris::soft_drop)
    .def("fall", &Tetris::fall)
    .def("getHold", &Tetris::getHold)
    .def("getNexts", &Tetris::getNexts)
    .def("shadow_image", &Tetris::shadow_image)
    .def("getScore", &Tetris::getScore)
    .def("recive_lines", &Tetris::recive_lines)    
    .def("holes", &Tetris::holes)
    .def("bumpiness", &Tetris::bumpiness)
    .def("total_height", &Tetris::total_height)
    .def("get_state", &Tetris::get_state)
    .def("getDone", &Tetris::getDone)
    ;
    //pieces data vector<vector<vector<vector<int>>>>
    handle.def("init_tetrominoes", &init_tetrominoes);
}