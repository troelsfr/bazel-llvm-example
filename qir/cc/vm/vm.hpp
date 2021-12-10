#pragma once
#include "qir/cc/llvm/llvm.hpp"

#include <iostream>
#include <memory>
namespace compiler {
class TestManager : public llvm::SectionMemoryManager
{
public:
  using SectionMemoryManager::SectionMemoryManager;
  using JITSymbol = llvm::JITSymbol;

  JITSymbol findSymbol(std::string const &name) override
  {
    std::cout << "Getting " << name << std::endl;
    return llvm::SectionMemoryManager::findSymbol(name);
  }
};

namespace {
void test_print_number(int64_t x)
{
  std::cout << "XXXXX: " << x << "\n";
}
}  // namespace

class JitEngine
{
public:
  using ExecutionSession              = llvm::orc::ExecutionSession;
  using RTDyldObjectLinkingLayer      = llvm::orc::RTDyldObjectLinkingLayer;
  using IRCompileLayer                = llvm::orc::IRCompileLayer;
  using DataLayout                    = llvm::DataLayout;
  using MangleAndInterner             = llvm::orc::MangleAndInterner;
  using ThreadSafeContext             = llvm::orc::ThreadSafeContext;
  using JITTargetMachineBuilder       = llvm::orc::JITTargetMachineBuilder;
  using ExpectedJitEngine             = llvm::Expected<std::unique_ptr<JitEngine>>;
  using SectionMemoryManager          = llvm::SectionMemoryManager;
  using ConcurrentIRCompiler          = llvm::orc::ConcurrentIRCompiler;
  using LLVMContext                   = llvm::LLVMContext;
  using Error                         = llvm::Error;
  using Module                        = llvm::Module;
  using JITEvaluatedSymbol            = llvm::JITEvaluatedSymbol;
  using ExpectedJitSymbol             = llvm::Expected<JITEvaluatedSymbol>;
  using StringRef                     = llvm::StringRef;
  using DynamicLibrarySearchGenerator = llvm::orc::DynamicLibrarySearchGenerator;
  using ThreadSafeModule              = llvm::orc::ThreadSafeModule;
  using JITDylib                      = llvm::orc::JITDylib;
  using ResourceTrackerSP             = llvm::orc::ResourceTrackerSP;
  using ExpectedThreadSafeModule      = llvm::Expected<ThreadSafeModule>;
  using MaterializationResponsibility = llvm::orc::MaterializationResponsibility;
  using TargetProcessControl          = llvm::orc::TargetProcessControl;
  using SymbolStringPool              = llvm::orc::SymbolStringPool;
  using SelfTargetProcessControl      = llvm::orc::SelfTargetProcessControl;

  JitEngine(std::unique_ptr<TargetProcessControl> TPC, std::unique_ptr<ExecutionSession> ES,
            JITTargetMachineBuilder JTMB, DataLayout DL)
    : TPC(std::move(TPC))
    , ES(std::move(ES))
    , DL(std::move(DL))
    , Mangle(*this->ES, this->DL)
    , ObjectLayer(*this->ES, []() { return std::make_unique<TestManager>(); })
    , compile_layer_(*this->ES, ObjectLayer,
                     std::make_unique<ConcurrentIRCompiler>(std::move(JTMB)))
    , MainJD(this->ES->createBareJITDylib("<main>"))
  {
    /// EXAMPLE: How to dynamically create a runtime
    // See https://stackoverflow.com/questions/57612173/llvm-jit-symbols-not-found?rq=1
    llvm::orc::SymbolMap M;
    // Register every symbol that can be accessed from the JIT'ed code.
    M[Mangle("print_number")] = JITEvaluatedSymbol(
        llvm::pointerToJITTargetAddress(&test_print_number), llvm::JITSymbolFlags());

    MainJD.define(absoluteSymbols(M));
    //    MainJD.addGenerator(cantFail(ES->getJITDylibByName("<main>")->define(absoluteSymbols(M))));
    /// END OF EXAMPLE

    MainJD.addGenerator(
        cantFail(DynamicLibrarySearchGenerator::GetForCurrentProcess(DL.getGlobalPrefix())));
  }

  ~JitEngine()
  {
    if (auto Err = ES->endSession())
      ES->reportError(std::move(Err));
  }

  static ExpectedJitEngine createNew()
  {
    auto SSP = std::make_shared<SymbolStringPool>();
    auto TPC = SelfTargetProcessControl::Create(SSP);
    if (!TPC)
      return TPC.takeError();

    auto ES = std::make_unique<ExecutionSession>(std::move(SSP));

    JITTargetMachineBuilder JTMB((*TPC)->getTargetTriple());

    auto DL = JTMB.getDefaultDataLayoutForTarget();
    if (!DL)
      return DL.takeError();

    return std::make_unique<JitEngine>(std::move(*TPC), std::move(ES), std::move(JTMB),
                                       std::move(*DL));
  }

  const DataLayout &getDataLayout() const
  {
    return DL;
  }

  JITDylib &getMainJITDylib()
  {
    return MainJD;
  }

  Error addModule(ThreadSafeModule TSM, ResourceTrackerSP RT = nullptr)
  {
    if (!RT)
    {
      RT = MainJD.getDefaultResourceTracker();
    }

    return compile_layer_.add(RT, std::move(TSM));
  }

  ExpectedJitSymbol lookup(StringRef Name)
  {
    return ES->lookup({&MainJD}, Mangle(Name.str()));
  }

private:
  std::unique_ptr<TargetProcessControl> TPC;
  std::unique_ptr<ExecutionSession>     ES;

  DataLayout        DL;
  MangleAndInterner Mangle;

  RTDyldObjectLinkingLayer ObjectLayer;
  IRCompileLayer           compile_layer_;

  JITDylib &MainJD;
};
}  // namespace compiler