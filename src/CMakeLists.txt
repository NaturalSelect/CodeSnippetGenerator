cmake_minimum_required(VERSION 3.14.0)

include(FetchContent)

#find source files
file(GLOB_RECURSE src "${PROJECT_SOURCE_DIR}/src" "*.h" "*.c")

if(MSVC)
    add_definitions("-D_CRT_SECURE_NO_WARNINGS=1")
endif()

add_executable(CodeSnippetGeneraion ${src})

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(CodeSnippetGeneraion PUBLIC Threads::Threads)
endif()