project(test C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

file(GLOB_RECURSE SOURCE
    # ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

add_definitions(-DTEST)

# SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage -Wl,--wrap=time
# ")
SET(CMAKE_C_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--wrap=open,--wrap=write,--wrap=close")
target_link_libraries(${PROJECT_NAME}
# gtest_main gtest gmock 
cmocka::cmocka)
# add_test(NAME all_test COMMAND ${PROJECT_NAME})
