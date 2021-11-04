import os, sys
# import sys

import CPPchessai

# Documentation
# help(CPPchessai) # get info about what the function does


"""struct Pet {
    Pet(const std::string &name) : name(name) { }
        void setName(const std::string &name_) { name = name_; }
        const std::string &getName() const { return name; }
        std::string name;
    };
    struct Dog : Pet {
        Dog(const std::string &name) : Pet(name) { }
        std::string bark() const { return "woof!"; }
    };

namespace py = pybind11;       [now we can add attributes in python even if they are not in class]
                                [|]
PYBIND11_MODULE(example, m) {   [v]
    py::class_<Pet>(m, "Pet", py::dynamic_attr())
        .def(py::init<const std::string &>())  [<- constructor ]
        .def("setName", &Pet::setName)moveGenerator::board_.GetPiece(position).getType()
        .def("getName", &Pet::getName)
        .def_readwrite("name", &Pet::name); [<- We can also directly expose the name field using the ]
                    [v specify C++ parent type]
    py::class_<Dog, Pet>(m, "Dog")
    .def(py::init<const std::string &>())
    .def("bark", &Dog::bark);
}
_____
p = example.Pet("Molly")
p.setName("Charly")
p.getName()
"""
