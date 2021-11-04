#include <pybind11/pybind11.h>
#include "chessai/chessAi.cpp"

namespace py = pybind11;

using namespace pybind11::literals;

PYBIND11_MODULE(CPPchessai, module) {
    module.doc() = "pybind11 example plugin"; // optional module docstring
    module.def("add", &add, "A function which adds two numbers", py::arg("i"), py::arg("j"));
}