#include "qir/cc/qir-module/scope-builder.hpp"

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
  typed_value.def("__eq__",
                  [](TypedValuePrototypePtr &a, TypedValuePrototypePtr &b) { return a == b; });

  auto builder = py::class_<ScopeBuilder, ScopeBuilderPtr>(module, "ScopeBuilder");
  builder.def("to_int8", &ScopeBuilder::toInt8)
      .def("to_int16", &ScopeBuilder::toInt16)
      .def("to_int32", &ScopeBuilder::toInt32)
      .def("to_int64", &ScopeBuilder::toInt64)
      .def("constant_array", &ScopeBuilder::constantArray)
      .def("constant_get_element", &ScopeBuilder::constantGetElement)
      .def("return_value", &ScopeBuilder::returnValue)
      .def("if_statement", &ScopeBuilder::ifStatement)
      .def("new_heap_variable", &ScopeBuilder::newHeapVariable)
      .def("new_heap_array", &ScopeBuilder::newHeapArray)
      .def("new_stack_variable", &ScopeBuilder::newStackVariable)
      .def("new_stack_array", &ScopeBuilder::newStackArray)
      .def("allocate_qubit", &ScopeBuilder::allocateQubit)
      .def("x", &ScopeBuilder::x)
      .def("z", &ScopeBuilder::z)
      .def("cnot", &ScopeBuilder::cnot);

  auto else_stmt = py::class_<ElseStatement, ElseStatementPtr>(module, "ElseStatement", builder);
  auto if_stmt   = py::class_<IfStatement, IfStatementPtr>(module, "IfStatement", else_stmt)
                     .def("else_statement", &IfStatement::elseStatement);

  py::class_<ScriptBuilder>(module, "ScriptBuilder")
      .def(py::init<>())
      .def("finalise", &ScriptBuilder::finalise)
      .def("new_function", &ScriptBuilder::newFunction)
      .def("get_type", &ScriptBuilder::getType)
      .def("get_qir", &ScriptBuilder::getQir);
}