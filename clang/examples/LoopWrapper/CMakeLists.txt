set(LLVM_LINK_COMPONENTS Support)

add_clang_executable(LoopWrapper LoopWrapper.cpp)

clang_target_link_libraries(LoopWrapper
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangLex
  clangRewrite
  clangSerialization
  clangTooling
  )