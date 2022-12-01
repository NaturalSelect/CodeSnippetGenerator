cmake_minimum_required(VERSION 3.14.0)
project(CodeSnippetGeneraion VERSION 1.0.0)

include(CTest)
enable_testing()

#C 11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED on)

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)