// RUN: HiddenCastCounter %s -- | FileCheck %s

void test_ints() {
    int a = 10;
    long b = a;   
    short c = a;
    unsigned int d = a;
}

// CHECK: Function test_ints
// CHECK-DAG: int -> long: 1
// CHECK-DAG: int -> short: 1
// CHECK-DAG: int -> unsigned int: 1