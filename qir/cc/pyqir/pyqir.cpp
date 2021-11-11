#include "qir/cc/qir-module/qir-module.hpp"

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
PYBIND11_MODULE(pyqir, module)
{
  module.doc() = "pyqir";

  py::class_<Qubit>(module, "Qubit");

  py::class_<QirBuilder>(module, "QirBuilder")
      .def(py::init<std::string>())
      .def("allocate_qubit", &QirBuilder::allocateQubit)
      .def("x", &QirBuilder::x)
      .def("z", &QirBuilder::z)
      .def("cnot", &QirBuilder::cnot)
      .def("get_qir", &QirBuilder::getQir);
}