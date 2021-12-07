import pyqir

program = pyqir.QirProgram()
i8 = program.get_type("Int8")
i64 = program.get_type("Int64")

builder = program.new_function("TestFunction", i64, [i8, i64])

q1 = builder.allocate_qubit()
q2 = builder.allocate_qubit()
x = builder.to_int64(127)
y = builder.to_int64(15)
idx = builder.to_int8(0)

# z = builder.add(x, y)
hello = builder.new_stack_variable(i64, "hello")
harr = builder.new_heap_array(i64, x, "heaparr")
arr = builder.new_stack_array(i64, x, "arr")

hello.set(y)
arr[y] = x

builder.x(q2)
builder.z(q1)
builder.cnot(q1, q2)

v = hello.get()
v = arr[y]

# print("A")
#arr = builder.constant_array(i8, [x, y])
# print("B")
#ret = builder.constant_get_element(arr, idx)
# print("C")
builder.return_value(v + x + y)


print(program.get_qir())
