cmake_minimum_required(VERSION 2.5)
project(RayTracingTheNextWeek)

set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(./include ./external)
aux_source_directory(./src SOURCES_FILES)

add_executable(Canvas ${SOURCES_FILES})
