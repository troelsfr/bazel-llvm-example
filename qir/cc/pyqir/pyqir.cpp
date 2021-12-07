#include "qir/cc/qir-module/qir-builder.hpp"

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
PYBIND11_DECLARE_HOLDER_TYPE(T, compiler::ValueContianer<T>);

PYBIND11_MODULE(pyqir, module)
{
  // https://stackoverflow.com/questions/20944533/when-should-i-use-constantdataarray-versus-constantarray
  // https://www.stephendiehl.com/llvm/
  //  py::class_<Function>(module, "Function").def(py::init<std::string>()).def("body",
  //  Function::body);

  module.doc() = "pyqir";
  using namespace compiler;

  py::class_<QirType>(module, "QirType");

  py::class_<Qubit>(module, "Qubit");
  auto typed_value =
      py::class_<TypedValuePrototype, TypedValuePrototypePtr>(module, "TypedValuePrototype");

  py::class_<ConstantInteger, ConstantIntegerPtr>(module, "ConstantInt", typed_value);
  py::class_<ConstantArray, ConstantArrayPtr>(module, "ConstantArray", typed_value);
  py::class_<TypedValue, TypedValuePtr>(module, "TypedValue", typed_value);

  py::class_<MutableStackVariable, MutableStackVariablePtr>(module, "MutableStackVariable",
                                                            typed_value)
      .def("set", &MutableStackVariable::set)
      .def("get", &MutableStackVariable::get);
  py::class_<MutableStackArray, MutableStackArrayPtr>(module, "MutableStackArray", typed_value)
      .def("__setitem__", &MutableStackArray::set)
      .def("__getitem__", &MutableStackArray::get);

  py::class_<MutableHeapVariable, MutableHeapVariablePtr>(module, "MutableHeapVariable",
                                                          typed_value)
      .def("set", &MutableHeapVariable::set)
      .def("get", &MutableHeapVariable::get);
  py::class_<MutableHeapArray, MutableHeapArrayPtr>(module, "MutableHeapArray", typed_value)
      .def("__setitem__", &MutableHeapArray::set)
      .def("__getitem__", &MutableHeapArray::get);

  typed_value.def("__add__",
                  [](TypedValuePrototypePtr &a, TypedValuePrototypePtr &b) { return a + b; });
  typed_value.def("__mul__",
                  [](TypedValuePrototypePtr &a, TypedValuePrototypePtr &b) { return a * b; });

  py::class_<QirBuilder, QirBuilderPtr>(module, "QirBuilder")
      .def("to_int8", &QirBuilder::toInt8)
      .def("to_int16", &QirBuilder::toInt16)
      .def("to_int32", &QirBuilder::toInt32)
      .def("to_int64", &QirBuilder::toInt64)
      .def("constant_array", &QirBuilder::constantArray)
      .def("constant_get_element", &QirBuilder::constantGetElement)
      .def("return_value", &QirBuilder::returnValue)
      .def("new_heap_variable", &QirBuilder::newHeapVariable)
      .def("new_heap_array", &QirBuilder::newHeapArray)
      .def("new_stack_variable", &QirBuilder::newStackVariable)
      .def("new_stack_array", &QirBuilder::newStackArray)
      .def("allocate_qubit", &QirBuilder::allocateQubit)
      .def("x", &QirBuilder::x)
      .def("z", &QirBuilder::z)
      .def("cnot", &QirBuilder::cnot);

  py::class_<QirProgram>(module, "QirProgram")
      .def(py::init<>())
      .def("new_function", &QirProgram::newFunction)
      .def("get_type", &QirProgram::getType)
      .def("get_qir", &QirProgram::getQir);
}