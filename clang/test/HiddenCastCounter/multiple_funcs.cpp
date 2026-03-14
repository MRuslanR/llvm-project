// RUN: HiddenCastCounter %s -- | FileCheck %s

void func1() {
    char c = 'a';
    int i = c;
    int j = i; 
}

void func2() {
    double d = 1.0;
    float f = d;
}

// CHECK: Function func1
// CHECK: char -> int: 1
// CHECK-NOT: int -> int

// CHECK: Function func2
// CHECK: double -> float: 1