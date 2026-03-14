# Loop Wrapper

LoopWrapper — инструмент на базе Clang AST, который находит все циклы в коде (for, while, do-while) и оборачивает их в вызовы loop_start(); и loop_end();

## Как собрать
mkdir build && cd build
cmake ../llvm -DLLVM_ENABLE_PROJECTS="clang" -DCLANG_BUILD_EXAMPLES=ON
make LoopWrapper

## Как запустить (из билда):
bin/LoopWrapper path/to/your_file --

## Как запустить тесты (из билда)
из билда:
make FileCheck (если не сбилдена)
./bin/llvm-lit -v ../clang/test/LoopWrapper/