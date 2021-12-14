#pragma once
#include "qir/cc/llvm/llvm.hpp"
#include "qir/cc/runtime/runtime.hpp"
#include "qir/cc/vm/jit-engine.hpp"
#include "qir/cc/vm/script.hpp"

#include <iostream>
#include <vector>

namespace compiler {

class VM
{
public:
  using String = std::string;

  // TODO: Make runtime copyable?
  VM(Runtime &runtime)
    : runtime_{runtime}
  {}

  template <typename R, typename... Args>
  R execute(Script const &script, String const &name, Args &&...args)
  {
    typedef R (*CallType)(Args...);
    // Loading the
    auto context = std::make_unique<llvm::LLVMContext>();
    auto module  = script.load(context.get());

    //
    llvm::ExitOnError          exit_on_error;
    std::unique_ptr<JitEngine> jit_engine = exit_on_error(JitEngine::createNew(runtime_));
    module->setDataLayout(jit_engine->getDataLayout());

    auto RT  = jit_engine->getMainJITDylib().createResourceTracker();
    auto TSM = llvm::orc::ThreadSafeModule(std::move(module), std::move(context));
    exit_on_error(jit_engine->addModule(std::move(TSM), RT));
    // TODO: Reinitialize the Runtime thingie InitializeModule();

    // Get the anonymous expression's JITSymbol.
    auto Sym = exit_on_error(jit_engine->lookup(name));

    // Get the symbol's address and cast it to the right type (takes no
    // arguments, returns a double) so we can call it as a native function.

    auto *function = (CallType)(intptr_t)Sym.getAddress();
    auto  ret      = function(std::forward<Args>(args)...);

    // Delete the anonymous expression module from the JIT.
    //  llvm::errs() << "Before RT remove\n";
    exit_on_error(RT->remove());
    //  llvm::errs() << "Exiting\n";
    return ret;
  }

private:
  Runtime &runtime_;
};

}  // namespace compiler