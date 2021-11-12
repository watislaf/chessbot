#include <pybind11/pybind11.h>
#include <chessAi.h>

namespace py = pybind11;

PYBIND11_MODULE(MY_WRAPPER, module) {
  py::class_<ChessAi>(module, "ChessAi", py::dynamic_attr())
      .def(py::init<std::string>())
      .def("startNewGame", &ChessAi::startNewGame)
      .def("applyMoveParams", &ChessAi::applyMoveParams)
      .def("getPossibleMovesForPosition", &ChessAi::getPossibleMovesForPosition)
      .def("getFenStr", &ChessAi::getFenStr)
      .def("getBoardStr", &ChessAi::getBoardStr)
      .def("isMoveExists", &ChessAi::isMoveExists)
      .def("whosMove", &ChessAi::whosMove)
      .def("getBestMoveStr", &ChessAi::getBestMoveStr);
}
