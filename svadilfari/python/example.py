import pysvadilfari

rt = pysvadilfari.Runtime()
rt.define_type(pysvadilfari.PrimitiveTypes.Int64, "Int64")
rt.define_type(pysvadilfari.PrimitiveTypes.Int8, "Int8")
rt.define_type(pysvadilfari.PrimitiveTypes.Int32, "Int32")
rt.define_type(pysvadilfari.PrimitiveTypes.Void, "Unit")

rt.init_platform()
rt.declare_function("__quantum__qis__cnot__body", "Int8", ["Int8"])

# EXPERIMENTAL CODE
# from ctypes import CFUNCTYPE, POINTER, c_int, c_void_p, c_long
# @CFUNCTYPE(c_void_p, c_long)
# def print_int(a):
#     print("Value is", a)
# rt.define_function("print", "Void", ["Int64"], print_int)

program = pysvadilfari.ScriptBuilder(rt)

i8 = program.get_type("Int8")
i32 = program.get_type("Int32")
i64 = program.get_type("Int64")

builder = program.new_function("TestFunction", "Int64", ["Int8", "Int64"])
x = builder.to_int64(127)
y = builder.to_int64(15)
idx = builder.to_int32(1)
hello = builder.new_stack_variable(i64, "hello")
harr = builder.new_heap_array(i64, x, "heaparr")
arr = builder.new_stack_array(i64, x, "arr")
hvar = builder.new_heap_variable(i64, "hvar")
hvar.set(x)


hello.set(y)
harr[y] = x
arr[y] = x
v = hello.get()
v = arr[y]
# print("A")
arr2 = builder.constant_array(i64, [x, y])
# print("B")
ret = builder.constant_get_element(arr2, idx)
# print("C")
# builder.return_value(v + x + y + hvar.get() + harr[y])
iff = builder.if_statement(x == x)
iff.new_stack_variable(i64, "hello")
otherwise = iff.else_statement()
zz = otherwise.new_stack_variable(i64, "zz")
# TODO: Unintuitive API, why is `get` needed?
iff2 = otherwise.if_statement(zz.get() == otherwise.to_int64(299))
iff2.else_statement()
builder.return_value(ret)

builder = program.new_function("BlahBlah", "Int64", ["Int32", "Int8", "Int64"])
builder.return_value(x)

builder = program.new_function("Test", "Unit", [])
x = builder.to_int64(127)
fnc = program.get_function_by_name("print")
builder.call(fnc, [x])

print(program.get_ir())

vm = pysvadilfari.VM(rt)
script = program.make_script()
vm.execute(script, "Test")
