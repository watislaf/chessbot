#include <pybind11/pybind11.h>
#include <chessAi.h>

namespace py = pybind11;

PYBIND11_MODULE(MY_WRAPPER, module) {
  py::class_<FEN>(module, "FEN", py::dynamic_attr()).
      def(py::init<const std::string&>());

  py::class_<ChessAi>(module, "ChessAi", py::dynamic_attr())
      .def(py::init<>())
      .def("startNewGame", &ChessAi::startNewGame)
      .def("applyMove",&ChessAi::applyMove)
      .def("getPossibleMovesForPosition", &ChessAi::getPossibleMovesForPosition)
      .def("getFenStr", &ChessAi::getFenStr);
}
