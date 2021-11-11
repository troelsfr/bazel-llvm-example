import pyqir

builder = pyqir.QirBuilder("TestFunction")

q1 = builder.allocate_qubit()
q2 = builder.allocate_qubit()
builder.x(q2)
builder.z(q1)
builder.cnot(q1, q2)

print(builder.get_qir())
