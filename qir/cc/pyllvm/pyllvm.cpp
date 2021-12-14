#include "qir/cc/qir-module/scope-builder.hpp"
#include "qir/cc/runtime/runtime.hpp"
#include "qir/cc/vm/script.hpp"

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
PYBIND11_DECLARE_HOLDER_TYPE(T, compiler::ValueContianer<T>);

PYBIND11_MODULE(pyllvm, module)
{
  // https://stackoverflow.com/questions/20944533/when-should-i-use-constantdataarray-versus-constantarray
  // https://www.stephendiehl.com/llvm/
  //  py::class_<Function>(module, "Function").def(py::init<std::string>()).def("body",
  //  Function::body);

  module.doc() = "pyllvm";
  using namespace compiler;

  py::class_<TypeDeclaration>(module, "TypeDeclaration");
  py::class_<TypeDeclarationRegister>(module, "TypeDeclarationRegister");

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
      .def("allocate_qubit", &ScopeBuilder::allocateQubit);

  auto else_stmt = py::class_<ElseStatement, ElseStatementPtr>(module, "ElseStatement", builder);
  auto if_stmt   = py::class_<IfStatement, IfStatementPtr>(module, "IfStatement", else_stmt)
                     .def("else_statement", &IfStatement::elseStatement);

  py::enum_<RuntimeDefinition::PrimitiveTypes>(module, "PrimitiveTypes")
      .value("Void", RuntimeDefinition::PrimitiveTypes::TYPE_VOID)
      .value("Bool", RuntimeDefinition::PrimitiveTypes::TYPE_BOOL)
      .value("Int8", RuntimeDefinition::PrimitiveTypes::TYPE_INT8)
      .value("Int16", RuntimeDefinition::PrimitiveTypes::TYPE_INT16)
      .value("Int32", RuntimeDefinition::PrimitiveTypes::TYPE_INT32)
      .value("Int64", RuntimeDefinition::PrimitiveTypes::TYPE_INT64)
      .value("Uint8", RuntimeDefinition::PrimitiveTypes::TYPE_UINT8)
      .value("Uint16", RuntimeDefinition::PrimitiveTypes::TYPE_UINT16)
      .value("Uint32", RuntimeDefinition::PrimitiveTypes::TYPE_UINT32)
      .value("Uint64", RuntimeDefinition::PrimitiveTypes::TYPE_UINT64)
      .value("Float32", RuntimeDefinition::PrimitiveTypes::TYPE_FLOAT32)
      .value("Float64", RuntimeDefinition::PrimitiveTypes::TYPE_FLOAT64)
      .export_values();

  auto rt_def =
      py::class_<RuntimeDefinition>(module, "RuntimeDefinition")
          .def("define_type",
               (void(RuntimeDefinition::*)(RuntimeDefinition::PrimitiveTypes const &,
                                           std::string const &))(&RuntimeDefinition::defineType));
  auto rt = py::class_<Runtime>(module, "Runtime", rt_def).def(py::init<>());

  py::class_<Script>(module, "Script");
  py::class_<ScriptBuilder>(module, "ScriptBuilder")
      .def(py::init<RuntimeDefinition const &>())
      .def("finalise", &ScriptBuilder::finalise)
      .def("new_function", &ScriptBuilder::newFunction)
      .def("get_type",
           [](ScriptBuilder const &builder, std::string const &name) -> TypeDeclaration {
             return builder.getType(name);
           })
      .def("get_qir", &ScriptBuilder::getQir);
}