cmake_minimum_required(VERSION 3.21)

add_subdirectory(util)
add_subdirectory(math)

target_sources(${COMMON_OBJLIB} PRIVATE
Common.c
Destruct.c
ArgCheck.h
Common.h
Destruct.h
Memory.h
Misc.h
Settings.h
VLA.h
Version.h
Version.h.in
)