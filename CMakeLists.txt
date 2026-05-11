cmake_minimum_required(VERSION 3.10)
project(MiniShellProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.cpp
    src/arithmetic.cpp
    src/read.cpp
    src/command.cpp
    src/if_while.cpp
    src/test.cpp
    src/subsidiary.cpp
    src/variables.cpp
)

set(LAUNCH_FILES
    src/launch.cpp
    src/subsidiary.cpp
)

add_executable(minishell ${SOURCE_FILES})
add_executable(launch ${LAUNCH_FILES})

target_include_directories(minishell PRIVATE include)
target_include_directories(launch PRIVATE include)