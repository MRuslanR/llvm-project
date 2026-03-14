set(LLVM_LINK_COMPONENTS Support)

add_clang_executable(HiddenCastCounter HiddenCastCounter.cpp)

clang_target_link_libraries(HiddenCastCounter
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  )