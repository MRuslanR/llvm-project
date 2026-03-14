// RUN: HiddenCastCounter %s -- | FileCheck %s

void test_floats() {
    float f = 3.14f;
    double d = f;
    int i = f;
    f = i;
}

// CHECK: Function test_floats
// CHECK-DAG: float -> double: 1
// CHECK-DAG: float -> int: 1
// CHECK-DAG: int -> float: 1