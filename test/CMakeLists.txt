project(test CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/deps/cmocka/include
)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/fib.cpp
    ${CMAKE_SOURCE_DIR}/src/write_file.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fib_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sys_mock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/write_file_test.cpp
)

add_definitions(-DTEST)

SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--wrap=open,--wrap=write,--wrap=close")
target_link_libraries(${PROJECT_NAME}
    gtest
    gmock
    pthread
    cmocka
)
