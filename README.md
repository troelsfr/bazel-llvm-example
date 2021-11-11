# LLVM-based Python module

In this repository we demonstrate how to build Python module relying on C++, LLVM and PyBind.

## Prerequisite

Install bazel version 3.5.0. Bazel is available on Windows via [chocolatey](https://community.chocolatey.org/packages/bazel) and Windows instructions can be found [here](https://docs.bazel.build/versions/main/install-windows.html). However, this is untested.

## How to run the example

Run

```
bazel run //qir/python:example
```

to build all dependencies of `qir/python/example.py` and run the executable. This commands builds a Rust library for logging, a C++ module (that uses the Rust library) which depends on LLVM to make a QIR builder and the C++ library is exported as a Python library. This Python library is imported in the python executable where it is used to build a QIR. A full build (including building required LLVM modules from scratch) is around 13 minutes. An example build output is here:

```
% bazel run //qir/python:example
Starting local Bazel server and connecting to it...
INFO: Analyzed target //qir/python:example (46 packages loaded, 2558 targets configured).
INFO: Found 1 target...
INFO: Deleting stale sandbox base /private/var/tmp/_bazel_tfr/eb219285a319690cc062d785a8a7d1bd/sandbox
INFO: From Linking qir/cc/pyqir/pyqir.so:
Target //qir/python:example up-to-date:
  bazel-bin/qir/python/example
INFO: Elapsed time: 816.845s, Critical Path: 103.30s
INFO: 1091 processes: 1091 darwin-sandbox.
INFO: Build completed successfully, 1143 total actions
INFO: Build completed successfully, 1143 total actions
```

Output from run:

```
Rust logger: Allocating qubit
Rust logger: Allocating qubit
Rust logger: Applying X
Rust logger: Applying Z
Rust logger: Applying CNOT
; ModuleID = 'qir.ll'
source_filename = "qir.ll"

%Qubit = type opaque

define i8 @TestFunction() {
entry:
  %0 = call %Qubit @__quantum__qis__qubit_create()
  %1 = call %Qubit @__quantum__qis__qubit_create()
  %2 = call %Qubit @__quantum__qis__x__body(%Qubit %1)
  %3 = call %Qubit @__quantum__qis__z__body(%Qubit %0)
  %4 = call %Qubit @__quantum__qis__cnot__body(%Qubit %0, %Qubit %1)
}

declare %Qubit @__quantum__qis__qubit_create()

declare %Qubit @__quantum__qis__x__body(%Qubit)

declare %Qubit @__quantum__qis__z__body(%Qubit)

declare %Qubit @__quantum__qis__cnot__body(%Qubit, %Qubit)

```

Other targets include:

```
bazel run //qir/rust/apps/hello-world:hello-world
```

To run all tests:

```
bazel test //...
```

Build all C++ targets:

```
bazel build //qir/cc/...
```

Test all C++ targets

```
bazel test //qir/cc/...
```

Build all Rust targets:

```
bazel build //qir/rust/...
```

Build C# `hello-world.exe`:

```
bazel run --host_platform @io_bazel_rules_dotnet//dotnet/toolchain:darwin_amd64_5.0.201 --platforms @io_bazel_rules_dotnet//dotnet/toolchain:darwin_amd64_5.0.201 //qir/csharp:hello-world.exe
```

Note that for C# we specify the host and target platforms. These settings can be put into the `.bazelrc.user` to avoid retyping in which case the run command reduces to

```
bazel run //qir/csharp:hello-world.exe
```

Clean up:

```
bazel clean
bazel shutdown
```

## Rust example

While not coupled to other frameworks, you can also run the rust example:

```
bazel run
```

## Testing

To run all tests, type

```
bazel //...
```

## Things to use

https://github.com/google/EXEgesis/tree/master/exegesis/util
