cmake_minimum_required(VERSION 3.21)

add_subdirectory(util)
add_subdirectory(test)
add_subdirectory(rdft)
add_subdirectory(cdft)
add_subdirectory(math)

target_sources(${TEST} PRIVATE
CommonTest.c
DftTest.c
Main.c
RegisterTest.c
CommonTest.h
DftTest.h
RegisterTest.h
Test.h
Test_.h
)