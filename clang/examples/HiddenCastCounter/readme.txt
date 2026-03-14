# Hidden Cast Counter

HiddenCastCounter - который ищет преобразование неявных типов внутри функций
Анализирует AST-дерево, находит скрытые преобразования между базовыми встроенными типами и выводит агрегированную стату по каждой функции

## Как собрать

cmake llvm -DCLANG_BUILD_EXAMPLES=ON 
make HiddenCastCounter

## Как запустить
llvm-project/build/bin/HiddenCastCounter  path/to/your/file --  

## Как запустить тесты
./bin/llvm-lit -v tools/clang/test/HiddenCastCounter/     