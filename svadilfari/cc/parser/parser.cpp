// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "ToyLexer.h"
#include "ToyParser.h"
#include "ToyParserBaseVisitor.h"
#include "ToyParserVisitor.h"
#include "antlr4-runtime.h"
#include "svadilfari/cc/llvm/llvm.hpp"
#include "svadilfari/cc/qir-module/scope-builder.hpp"
#include "svadilfari/cc/qir-module/script-builder.hpp"
#include "svadilfari/cc/runtime/runtime.hpp"
#include "svadilfari/cc/vm/jit-engine.hpp"
#include "svadilfari/cc/vm/script.hpp"
#include "svadilfari/cc/vm/vm.hpp"

#include <iostream>
#include <vector>

using namespace toylang;
using namespace compiler;

using namespace antlr4;

class ToyIrBuilder : public ToyParserVisitor
{
public:
  using Any         = antlrcpp::Any;
  using LLVMContext = llvm::LLVMContext;
  using Module      = llvm::Module;

  ToyIrBuilder(RuntimeDefinition const &runtime_definition)
    : program_{runtime_definition}
  {}

  virtual Any visitMain(ToyParser::MainContext *ctx) override
  {
    visitChildren(ctx);
    //    llvm::outs() << *module_ << "\n";
    return 1337;
  }

  virtual Any visitDivide(ToyParser::DivideContext *ctx) override
  {
    return visitChildren(ctx);
  }

  virtual Any visitAnd_(ToyParser::And_Context *ctx) override
  {
    return visitChildren(ctx);
  }

  virtual Any visitConquer(ToyParser::ConquerContext *ctx) override
  {
    return visitChildren(ctx);
  }

  virtual Any visitUnused(ToyParser::UnusedContext *ctx) override
  {
    return visitChildren(ctx);
  }

  virtual Any visitFunctionDeclaration(ToyParser::FunctionDeclarationContext *ctx) override
  {

    // Adding function arguments to the stack
    std::vector<std::string> arg_types;
    std::vector<std::string> arg_names;

    for (auto &arg : ctx->functionArguments()->singleArgumentDeclaration())
    {
      arg_types.push_back(arg->typeName()->getText());
      arg_names.push_back(arg->id()->getText());
    }

    auto function_name = ctx->ID()->getText();
    auto return_type   = ctx->typeName()->getText();

    pushFunctionBuilder(program_.newFunction(function_name, return_type, arg_types));

    // TODO: Replace call with getFunctionDeclaration(function_name)
    auto function_decl = program_.getOrDeclareFunction(function_name, return_type, arg_types);

    // Naming the arguments
    uint64_t i = 0;
    for (auto &arg : function_decl->args())
    {
      // Adding variable to the scope
      auto arg_type = program_.getType(arg_types[i]);
      auto arg_ref  = TypedValue::create(arg_type, builder()->builder(), &arg);
      builder()->scope().insert(arg_names[i], arg_ref);

      arg.setName(arg_names[i++]);
    }

    // Building statements
    auto statements = ctx->statement();
    for (auto &stmt : statements)
    {
      visit(stmt);
    }

    popFunctionBuilder();

    return 0;
  }

  virtual Any visitArrayInitializer(ToyParser::ArrayInitializerContext *context) override
  {
    std::vector<TypedValuePrototypePtr> initialiser_list;

    std::type_index type_id{std::type_index(typeid(void))};
    bool            first = true;
    for (auto &element : context->argumentInstances()->expression())
    {
      TypedValuePrototypePtr e = visit(element);

      if (first)
      {
        type_id = e->typeId();
        first   = false;
      }

      if (type_id != e->typeId())
      {
        throw std::runtime_error("Mixed type arrays are not supported.");
      }

      initialiser_list.push_back(e);
    }

    if (type_id == std::type_index(typeid(void)))
    {
      throw std::runtime_error("Cannot construct array with no type and no type hint.");
    }

    auto type_declaration = program_.getType(type_id);
    auto size             = builder()->toInt64(initialiser_list.size());
    auto ret              = builder()->newStackArray(type_declaration, size, "");

    int64_t i = 0;
    for (auto &value : initialiser_list)
    {
      ret->set(builder()->toInt64(i * type_declaration.size), value);
      ++i;
    }
    llvm::errs() << "Returning from array\n";
    return static_cast<TypedValuePrototypePtr>(ret);
  }

  virtual Any visitKvInstances(ToyParser::KvInstancesContext *context) override
  {
    throw std::runtime_error("not implemented");
  }

  virtual Any visitStructInitializer(ToyParser::StructInitializerContext *context) override
  {
    throw std::runtime_error("not implemented");
  }

  virtual Any visitKeyValuePair(ToyParser::KeyValuePairContext *context) override
  {
    throw std::runtime_error("not implemented");
  }

  virtual Any visitFunctionArguments(ToyParser::FunctionArgumentsContext *ctx) override
  {
    // return visitChildren(ctx);
    return 3;
  }

  virtual Any visitSingleArgumentDeclaration(
      ToyParser::SingleArgumentDeclarationContext *ctx) override
  {
    throw std::runtime_error("visitSingleArgumentDeclaration Not implemented");
    return visitChildren(ctx);
  }

  virtual Any visitUnused2(ToyParser::Unused2Context *ctx) override
  {
    throw std::runtime_error("visitUnused2 Not implemented");
    return visitChildren(ctx);
  }

  virtual Any visitStatement(ToyParser::StatementContext *ctx) override
  {
    return visitChildren(ctx);
  }

  virtual Any visitAssignment(ToyParser::AssignmentContext *ctx) override
  {
    TypedValuePrototypePtr lhs = visit(ctx->lhs);
    TypedValuePrototypePtr rhs = visit(ctx->rhs);
    lhs->writeValue(rhs->readValue());
    return lhs;
  }

  virtual Any visitDeclaration(ToyParser::DeclarationContext *ctx) override
  {
    llvm::errs() << "visitDecl: " << ctx->getText() << "\n";

    // Getting the value of the declaration
    TypedValuePrototypePtr ssa_expr;
    if (ctx->expression())
    {
      llvm::errs() << "Creating ssa"
                   << "\n";
      ssa_expr = visit(ctx->expression());
      llvm::errs() << "Got ssa"
                   << "\n";
    }

    auto name = ctx->id()->getText();
    if (ctx->MutVariableDecl())
    {
      MutableStackVariablePtr variable;

      if (ctx->typeName())
      {
        // TODO: Add support for arrays
        auto type_name = ctx->typeName()->getText();
        auto t         = program_.getType(type_name);

        // TODO: Check allocation type
        auto newexpr = builder()->newStackVariable(t, name);
        newexpr->set(ssa_expr);

        // TODO: Add variable to locals
        ssa_expr = newexpr;
      }
      else
      {
        throw std::runtime_error("Automatic type deduction not implemented yet.");
      }
    }

    if (ctx->ImutableVariableDecl())
    {
      llvm::errs() << "Constant\n";
      // builder().newStackVariable()
      // ssa_expr = runtime_definition_->getDefaultValue(ctx->typeName()->getText());
    }

    // Setting name and registering
    ssa_expr->readValue()->setName(name);
    if (builder()->scope().has(name))
    {
      throw std::runtime_error(name + " redeclares or shadows previously defined variable.");
    }

    builder()->scope().insert(name, ssa_expr);

    return ssa_expr;
  }

  virtual Any visitArgumentInstances(ToyParser::ArgumentInstancesContext *context) override
  {
    std::vector<TypedValuePrototypePtr> args;
    for (auto &arg : context->expression())
    {
      llvm::errs() << "xx\n";
      TypedValuePrototypePtr a = visit(arg);
      llvm::errs() << "yy\n";

      args.push_back(a);
    }
    llvm::errs() << "ret\n";

    return args;
  }

  virtual Any visitExpression(ToyParser::ExpressionContext *ctx) override
  {
    llvm::errs() << "visitExpr\n";

    if (ctx->op)
    {
      llvm::errs() << "A\n";
      TypedValuePrototypePtr lhs = visit(ctx->lhs);
      TypedValuePrototypePtr rhs = visit(ctx->rhs);
      llvm::errs() << "Op: \n";
      llvm::errs() << " - " << *lhs->readValue() << "\n";
      llvm::errs() << " - " << *rhs->readValue() << "\n";

      switch (ctx->op->getType())
      {
      case ToyParser::Plus:
        return static_cast<TypedValuePrototypePtr>(lhs + rhs);
        break;
      case ToyParser::Minus:
        return static_cast<TypedValuePrototypePtr>(lhs + rhs);  // TODO:
        break;
      case ToyParser::Multiply:
        return static_cast<TypedValuePrototypePtr>(lhs * rhs);
        break;
      case ToyParser::Divide:
        return static_cast<TypedValuePrototypePtr>(lhs * rhs);  // TODO:
        break;
      case ToyParser::Remainder:
        throw std::runtime_error("Remainder not implemented");
        break;
      default:
        throw std::runtime_error("Unknown not implemented");
        break;
      }

      throw std::runtime_error("Unknown operation");
    }
    else if (ctx->functionName)
    {
      llvm::errs() << "FUNCTION CALL: " << ctx->functionName->getText() << "\n";
      auto decl = program_.getFunctionByLlvmName(ctx->functionName->getText());
      if (decl.function == nullptr)
      {
        throw std::runtime_error("Function not declared.");
      }

      llvm::errs() << "before calling\n";

      std::vector<TypedValuePrototypePtr> args = visit(ctx->argumentInstances());
      llvm::errs() << "calling\n";
      TypedValuePrototypePtr ret = builder()->call(decl, args);
      return ret;
    }
    else if (ctx->arrayInitializer())
    {
      llvm::errs() << "Entering expression\n";
      auto x = visitArrayInitializer(ctx->arrayInitializer());
      llvm::errs() << "Was here?\n";

      TypedValuePrototypePtr ret = x;
      llvm::errs() << "Exiting expression\n";

      return ret;
    }
    else if (ctx->Integer())
    {
      auto text = ctx->Integer()->getText();
      llvm::errs() << "B: " << text << "\n";
      auto p = text.c_str();
      errno  = 0;
      char   *p_end;
      int64_t cst = std::strtol(p, &p_end, 10);
      // TODO: Add support for literals

      llvm::errs() << "Returning result\n";
      return static_cast<TypedValuePrototypePtr>(builder()->toInt64(cst));
    }
    else if (ctx->subexpression)
    {
      throw std::runtime_error("Subexpression not implemented");

      /*
      return visit(ctx->subexpression);
      */
    }
    else if (ctx->arrayAddress)
    {
      TypedValuePrototypePtr index   = visit(ctx->index);
      TypedValuePrototypePtr address = visit(ctx->arrayAddress);
      return address->getArrayElement(index);
    }
    else if (ctx->identifier)
    {
      auto name = ctx->identifier->getText();
      if (!builder()->scope().has(name))
      {
        throw std::runtime_error(name + " not declared.");
      }

      return builder()->scope().get(name);
    }
    else if (ctx->String())
    {
      throw std::runtime_error("Strings not implemented");
    }
    else
    {
      throw std::runtime_error("Other expressions not implemented");
    }

    //    throw std::runtime_error("Not implemented");
    return visitChildren(ctx);
  }

  virtual Any visitReturn(ToyParser::ReturnContext *ctx) override
  {
    if (ctx->expression())
    {
      TypedValuePrototypePtr ret = visitChildren(ctx);

      builder()->returnValue(ret);
      return nullptr;
    }
    // TODO: Explicitly return void
    // builder()->returnValue(visitChildren(ctx));
    return nullptr;
  }

  virtual Any visitContinue(ToyParser::ContinueContext *ctx) override
  {
    throw std::runtime_error("Not implemented");
    return visitChildren(ctx);
  }

  virtual Any visitTypeName(ToyParser::TypeNameContext *ctx) override
  {
    throw std::runtime_error("Not implemented");
    return visitChildren(ctx);
  }

  virtual Any visitId(ToyParser::IdContext *ctx) override
  {
    throw std::runtime_error("Id not implemented");
    return 0;
  }

  virtual Any visitArray(ToyParser::ArrayContext *ctx) override
  {
    throw std::runtime_error("visitArray Not implemented");
    return visitChildren(ctx);
  }

  virtual Any visitIdarray(ToyParser::IdarrayContext *ctx) override
  {
    throw std::runtime_error("Idarray Not implemented");

    return visitChildren(ctx);
  }

  virtual Any visitAny(ToyParser::AnyContext *ctx) override
  {
    throw std::runtime_error("Any Not implemented");

    return visitChildren(ctx);
  }

  void pushFunctionBuilder(ScopeBuilderPtr const &builder)
  {
    function_builders_.push_back(builder);
  }

  void popFunctionBuilder()
  {
    function_builders_.pop_back();
  }

  ScriptBuilder &program()
  {
    return program_;
  }

  ScopeBuilderPtr builder()
  {
    if (function_builders_.empty())
    {
      throw std::runtime_error("Cannot build outside of function scope");
    }
    return function_builders_.back();
  }

private:
  ScriptBuilder                program_;
  std::vector<ScopeBuilderPtr> function_builders_{};
};

class Compiler
{
public:
  using String = std::string;

  // TODO: In principle the compiler only depends on the runtime definitions and not the runtime
  // itself
  Compiler(RuntimeDefinition &runtime_definition)
    : runtime_definition_{runtime_definition}
  {}

  Script compile(String source, Script::Type const &type = Script::Type::LL_SCRIPT)
  {
    ANTLRInputStream input(source);

    ToyLexer          lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    ToyParser               parser(&tokens);
    ToyParser::MainContext *tree = parser.main();

    ToyIrBuilder visitor(runtime_definition_);

    //    visitor.program().getOrDeclareFunction("print", "Void", {"Int64"});
    //    visitor.program().getOrDeclareFunction("Qubit::print<Int64>", "Void", {"Int64"});

    visitor.visitMain(tree);

    return visitor.program().makeScript();
  }

private:
  RuntimeDefinition &runtime_definition_;
};

int main(int, const char **)
{
  Runtime runtime;

  runtime.defineType<int8_t>("Int8");
  runtime.defineType<int16_t>("Int16");
  runtime.defineType<int32_t>("Int32");
  runtime.defineType<int64_t>("Int64");
  runtime.defineType<void>("Void");

  runtime.defineType<Qubit>("Qubit");
  runtime.defineFunction("print", [](int64_t x) -> void { std::cout << "Value: " << x << "\n"; });
  runtime.defineFunction("hello<Int64>:Int64",
                         [](int64_t x) -> void { std::cout << "Value: " << x << "\n"; });

  Compiler compiler(runtime);

  auto script = compiler.compile(R"script(
operation test(z: Int64): Int64
{
  return 382 * z;
}

operation main(argc: Int64) : Int64 // Array< String >, Array< Int >
{
  mutable x : Int64 = 9  + argc;
  print(x);
  let y = 9 * 2 +  test(x);
  print(y);
  let arr = [y,2,3,4];
  print(arr[0]);
  arr[1] = 29 + arr[0];
  x = arr[1] * 3;
  print(arr[1]);
  print(x);
  arr[3] = x;
  print(arr[3]);
  return arr[3];
})script");

  runtime.initPlatform();
  VM   vm(runtime);
  auto ret = vm.execute<int64_t>(script, "main", 4);
  llvm::errs() << "Result = " << ret << "\n";

  // Script
  llvm::outs() << script.payload << "\n";

  return 0;
}
