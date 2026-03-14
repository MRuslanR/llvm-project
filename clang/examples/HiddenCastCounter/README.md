# Hidden Cast Counter

HiddenCastCounter - который ищет преобразование неявных типов внутри функций
Анализирует AST-дерево, находит скрытые преобразования между базовыми встроенными типами и выводит агрегированную стату по каждой функции

## Как собрать
mkdir build && cd build
cmake ../llvm -DLLVM_ENABLE_PROJECTS="clang" -DCLANG_BUILD_EXAMPLES=ON
make HiddenCastCounter

## Как запустить (из билда)
./bin/HiddenCastCounter path/to/your/file --

## Как запустить тесты (из билда)
(если нужно, то `make clang FileCheck`)
./bin/llvm-lit -v ../clang/test/HiddenCastCounter/