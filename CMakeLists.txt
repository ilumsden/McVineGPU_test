cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(McVineGPU LANGUAGES CXX CUDA)

option(BUILD_TESTING "Build tests." OFF)

file(GLOB cpu_source_files "${CMAKE_SOURCE_DIR}/src/*.cpp")
if ("${CMAKE_SOURCE_DIR}/src/main.cpp" IN_LIST cpu_source_files)
    message(STATUS "Setting main.cpp as driver.")
    list(REMOVE_ITEM cpu_source_files "${CMAKE_SOURCE_DIR}/src/main.cpp")
    set(DRIVER_SRC "${CMAKE_SOURCE_DIR}/src/main.cpp")
endif()
file(GLOB gpu_source_files "${CMAKE_SOURCE_DIR}/cuda/*.cu")
file(GLOB gpu_headers "${CMAKE_SOURCE_DIR}/include/*.hpp")
if(BUILD_TESTING)
    add_subdirectory(googletest)
    enable_testing()
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    include_directories(${CMAKE_SOURCE_DIR}/test/include)
    set(TEST_NAME ${PROJECT_NAME}_test)
    file(GLOB_RECURSE test_src "test/test_*.cpp")
    list(APPEND test_src "${CMAKE_SOURCE_DIR}/test/run_tests.cpp")
    file(GLOB_RECURSE test_support_src "test/*.cpp")
    foreach(fname IN LISTS test_src)
        if(fname IN_LIST test_support_src)
            list(REMOVE_ITEM test_support_src ${fname})
        endif()
    endforeach(fname)
    file(GLOB_RECURSE test_headers "test/*.hpp")
    file(GLOB_RECURSE test_cuda "test/*.cu")
endif()

message(STATUS "CUDA include directories are ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})

list(APPEND CMAKE_CUDA_FLAGS "-cudart static")

option(DEBUG "Triggers a full debug build." OFF)
option(PRINT1 "Builds the debug for the external intersection calculation" OFF)
option(PRINT2 "Builds the debug for the scattering site calculation" OFF)
option(PRINT3 "Builds the debug for the elastic scattering velocity calculation" OFF)
option(PRINT4 "Builds the debug for the internal intersection calculation" OFF)
option(RANDTEST "Builds the debug for the Spherical RNG." OFF)
option(INTERIORTEST "Builds the debug for checking the raw data from the interior intersection algorithm." OFF)

if(DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DDEBUG")
endif()
if(PRINT1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPRINT1")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DPRINT1")
endif()
if(PRINT2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPRINT2")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DPRINT2")
endif()
if(PRINT3)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPRINT3")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DPRINT3")
endif()
if(PRINT4)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPRINT4")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DPRINT4")
endif()
if(RANDTEST)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRANDTEST")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DRANDTEST")
endif()
if(INTERIORTEST)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DINTERIORTEST")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DINTERIORTEST")
endif()

add_library(cuMcVine STATIC ${gpu_source_files} ${gpu_headers} ${cpu_source_files})

target_compile_features(cuMcVine PUBLIC cxx_std_11)
set_target_properties(cuMcVine PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(cuMcVine curand)

if(DRIVER_SRC)
    message(STATUS "Making executable")
    add_executable(${PROJECT_NAME} ${DRIVER_SRC})
    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
    target_link_libraries(${PROJECT_NAME} PRIVATE cuMcVine)
endif()

if(BUILD_TESTING)
    message(STATUS "Building test suite.")
    add_library(testMcVineGPU STATIC ${test_headers} ${test_cuda} ${test_support_src})
    target_compile_features(testMcVineGPU PUBLIC cxx_std_11)
    set_target_properties(testMcVineGPU PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_link_libraries(testMcVineGPU cuMcVine)
    add_executable(${TEST_NAME} ${test_src})
    target_compile_features(${TEST_NAME} PUBLIC cxx_std_11)
    target_link_libraries(${TEST_NAME} testMcVineGPU)
    target_link_libraries(${TEST_NAME} gtest)
endif()
