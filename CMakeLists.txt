cmake_minimum_required(VERSION 3.6)
project(CI_Homework_4)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DNDEBUG -static -std=c99 -pipe")

set(SOURCE_FILES
        LICENSE "Problem 3/Problem_3.c")

add_executable(CI_Homework_4 ${SOURCE_FILES})

target_link_libraries(CI_Homework_4 m)
