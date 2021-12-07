; ModuleID = 'qir.ll'
source_filename = "qir.ll"

%Qubit = type opaque

define i64 @TestFunction(i8 %0, i64 %1) {
entry:
  %2 = call %Qubit* @__quantum__qis__qubit_create()
  %3 = call %Qubit* @__quantum__qis__qubit_create()
  %hello = alloca i64, align 8
  %arr = alloca i64, i64 127, align 8
  store i64 15, i64* %hello, align 4
  %4 = getelementptr i64, i64* %arr, i64 15
  store i64 127, i64* %4, align 4
  %5 = call %Qubit* @__quantum__qis__x__body(%Qubit* %3)
  %6 = call %Qubit* @__quantum__qis__z__body(%Qubit* %2)
  %7 = call %Qubit* @__quantum__qis__cnot__body(%Qubit* %2, %Qubit* %3)
  %8 = load i64, i64* %hello, align 4
  %9 = getelementptr i64, i64* %arr, i64 15
  %10 = load i64, i64* %9, align 4
  %11 = add i64 %10, 127
  %12 = add i64 %11, 15
  ret i64 %12
}

declare %Qubit* @__quantum__qis__qubit_create()

declare %Qubit* @__quantum__qis__x__body(%Qubit*)

declare %Qubit* @__quantum__qis__z__body(%Qubit*)

declare %Qubit* @__quantum__qis__cnot__body(%Qubit*, %Qubit*)

