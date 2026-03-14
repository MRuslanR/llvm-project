// RUN: LoopWrapper %s -- | FileCheck %s

void test_for() {
    // CHECK: loop_start();
    // CHECK-NEXT: for (int i = 0; i < 5; i++) {
    for (int i = 0; i < 5; i++) {
        int x = i;
    }
    // CHECK: }
    // CHECK-NEXT: loop_end();
}

void test_while() {
    int a = 0;
    // CHECK: loop_start();
    // CHECK-NEXT: while (a < 5) {
    while (a < 5) {
        a++;
    }
    // CHECK: }
    // CHECK-NEXT: loop_end();
}