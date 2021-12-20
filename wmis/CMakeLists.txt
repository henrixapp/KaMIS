cmake_minimum_required(VERSION 2.8.9)
project(components)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release) 
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-omit-frame-pointer -march=native -DNDEBUG -fPIC -O3 ")

CHECK_CXX_COMPILER_FLAG(-Wno-unused-value COMPILER_SUPPORTS_NOUNUSED)
if(COMPILER_SUPPORTS_NOUNUSED)
add_definitions(-Wno-unused-value)
endif()

CHECK_CXX_COMPILER_FLAG(-Wno-unused-value COMPILER_SUPPORTS_NOUNUSEDRES)
if(COMPILER_SUPPORTS_NOUNUSEDRES)
add_definitions(-Wno-unused-result)
endif()

CHECK_CXX_COMPILER_FLAG(-fno-stack-limit COMPILER_SUPPORTS_FNOSTACKLIMITS)
if(COMPILER_SUPPORTS_FNOSTACKLIMITS)
  add_definitions(-fno-stack-limit)
endif()

        CHECK_CXX_COMPILER_FLAG(-funroll-loops COMPILER_SUPPORTS_FUNROLL_LOOPS)
if(COMPILER_SUPPORTS_FUNROLL_LOOPS)
  add_definitions(-funroll-loops)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/app)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/argtable3-3.0.3)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/io)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/partition)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/tools)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/io)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/tools)






include_directories(
        app
        lib
        lib/data_structure
        lib/data_structure/priority_queues
        lib/mis
        lib/mis/ils
        lib/mis/initial_mis
        lib/mis/kernel
        lib/tools
)


set(SOURCES
        lib/tools/mis_log.cpp
        lib/mis/ils/ils.cpp
        lib/mis/ils/local_search.cpp
        lib/mis/initial_mis/greedy_mis.cpp
        lib/mis/initial_mis/greedy_vertex.cpp
        lib/mis/initial_mis/random_mis.cpp
        lib/mis/initial_mis/initial_mis.cpp
        lib/data_structure/mis_permutation.cpp
        lib/data_structure/candidate_list.cpp
        lib/data_structure/operation_log.cpp
        lib/data_structure/priority_queues/bucket_array.cpp
        lib/mis/kernel/branch_and_reduce_algorithm.cpp
        lib/mis/kernel/reductions.cpp
        ../lib/tools/mis_graph_io.cpp)

add_library(libsources OBJECT ${SOURCES})

add_executable(branch_reduce "app/branch_reduce.cpp" $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libsources>)
add_executable(weighted_ls "app/weighted_ls.cpp" $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libsources>)
add_executable(merge_graph_weights "app/merge_graph_weights.cpp" $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libsources>)


