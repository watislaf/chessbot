#include <pybind11/pybind11.h>
#include <chessEngine.h>

namespace py = pybind11;

PYBIND11_MODULE(WRAPPER_NAME, module) {
  py::class_<ChessEngine>(module, "ChessEngine", py::dynamic_attr())
      .def(py::init<std::string>())
      .def("startNewGame", &ChessEngine::startNewGame)
      .def("applyMoveParams", &ChessEngine::applyMoveParams)
      .def("getPossibleMovesForPosition", &ChessEngine::getPossibleMovesForPosition)
      .def("getBoardStr", &ChessEngine::getBoardStr)
      .def("isMoveExists", &ChessEngine::isMoveExists)
      .def("whosTurn", &ChessEngine::whosTurn)
      .def("getBestMoveStr", &ChessEngine::getBestMoveStr);
}
