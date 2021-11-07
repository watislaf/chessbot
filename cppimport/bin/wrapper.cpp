#include <pybind11/pybind11.h>
#include <chessAi.h>

namespace py = pybind11;

using namespace pybind11::literals;

PYBIND11_MODULE(wrap, module) {
  py::class_<FEN>(module, "FEN", py::dynamic_attr()).
      def(py::init<const std::string &>());

  py::class_<ChessAi>(module, "ChessAi", py::dynamic_attr())
      .def(py::init<>())
      .def("startNewGame", &ChessAi::startNewGame)
      .def("getPossibleMovesForPiece", &ChessAi::getPossibleMovesForPiece)
      .def("getFen",&ChessAi::getFen);
  //   module.doc() = "pybind11 example plugin"; // optional module docstring
//    module.def("add", &add, "A function which adds two numbers", py::arg("i"), py::arg("j"));
//  !!!
}
