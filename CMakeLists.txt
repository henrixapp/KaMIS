cmake_minimum_required(VERSION 3.10)
include(CheckCXXCompilerFlag)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_program(CCACHE_PROGRAM ccache)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(CCACHE_PROGRAM)
  message(STATUS "Using compiler cache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()
project(KaMIS C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# if no build mode is specified build in release mode
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# tweak compiler flags
CHECK_CXX_COMPILER_FLAG(-funroll-loops COMPILER_SUPPORTS_FUNROLL_LOOPS)
if(COMPILER_SUPPORTS_FUNROLL_LOOPS)
  add_definitions(-funroll-loops)
endif()
CHECK_CXX_COMPILER_FLAG(-fno-stack-limit COMPILER_SUPPORTS_FNOSTACKLIMITS)
if(COMPILER_SUPPORTS_FNOSTACKLIMITS)
  add_definitions(-fno-stack-limit)
endif()
#CHECK_CXX_COMPILER_FLAG(-Wall COMPILER_SUPPORTS_WALL)
#if(COMPILER_SUPPORTS_WALL)
  #add_definitions(-Wall)
#endif()
CHECK_CXX_COMPILER_FLAG(-march=native COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
  add_definitions(-march=native)
endif()
CHECK_CXX_COMPILER_FLAG(-fpermissive COMPILER_SUPPORTS_FPERMISSIVE)
if(COMPILER_SUPPORTS_FPERMISSIVE)
  add_definitions(-fpermissive)
endif()

CHECK_CXX_COMPILER_FLAG(-Wno-unused-value COMPILER_SUPPORTS_NOUNUSED)
if(COMPILER_SUPPORTS_NOUNUSED)
add_definitions(-Wno-unused-value)
endif()
CHECK_CXX_COMPILER_FLAG(-Wno-unused-value COMPILER_SUPPORTS_NOUNUSEDRES)
if(COMPILER_SUPPORTS_NOUNUSEDRES)
add_definitions(-Wno-unused-result)
endif()

# check dependencies
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP support detected")
  add_definitions(${OpenMP_CXX_FLAGS})
else()
  message(WARNING "OpenMP not available, activating workaround")
  add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
  set_property(TARGET OpenMP::OpenMP_CXX PROPERTY INTERFACE_COMPILE_OPTIONS "")
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/misc)
endif()

# 64 Bit option
option(64BITMODE "64 bit mode" OFF)
if(64BITMODE)
  add_definitions("-DMODE64BITEDGES")
  add_definitions("-DPOINTER64=1")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/argtable3-3.0.3)
add_subdirectory(wmis)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/app)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/argtable3-3.0.3)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/data_structure)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/io)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/tools)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/mis)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/mis/evolutionary)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/mis/evolutionary/combine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/mis/initial_mis)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/data_structure)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/io)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/tools)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/algorithms)

add_subdirectory(extern/KaHIP EXCLUDE_FROM_ALL)

set(LIBSOURCE_SOURCE_FILES
lib/tools/mis_log.cpp
lib/tools/mis_graph_io.cpp
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
lib/mis/evolutionary/population_mis.cpp
lib/mis/evolutionary/reduction_evolution.cpp
lib/mis/hopcroft/bipartite_vertex_cover.cpp
lib/mis/kernel/branch_and_reduce_algorithm.cpp
lib/mis/kernel/modified.cpp
lib/mis/evolutionary/separator_pool.cpp
lib/mis/evolutionary/combine/combine.cpp
lib/mis/evolutionary/combine/cover_combine.cpp
lib/mis/evolutionary/combine/separator_combine.cpp
lib/mis/evolutionary/combine/multiway_combine.cpp
lib/mis/kernel/ParFastKer/fast_reductions/src/full_reductions.cpp
lib/mis/kernel/ParFastKer/fast_reductions/src/MaximumMatching.cpp
lib/mis/kernel/ParFastKer/fast_reductions/src/parallel_reductions.cpp
lib/mis/kernel/ParFastKer/LinearTime/MIS_sigmod_pub/Graph.cpp
lib/mis/kernel/ParFastKer/LinearTime/MIS_sigmod_pub/Utility.cpp
)
add_library(libfiles OBJECT ${LIBSOURCE_SOURCE_FILES})
link_libraries(libfiles libkaffpa kahip)



set(LIBONLINEMIS_FILES
lib/mis/ils/online_ils.cpp
lib/mis/ils/local_search_online.cpp
lib/mis/initial_mis/greedy_mis_online.cpp
lib/data_structure/mis_permutation_online.cpp)
add_library(libonlinemis OBJECT ${LIBONLINEMIS_FILES})

add_executable(redumis app/reduction_evomis.cpp $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libfiles>)
target_link_libraries(redumis ${OpenMP_CXX_LIBRARIES})
install(TARGETS redumis DESTINATION bin)

#add_executable(graphchecker app/graphchecker.cpp $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libfiles>)
#target_link_libraries(graphchecker ${OpenMP_CXX_LIBRARIES})
#install(TARGETS graphchecker DESTINATION bin)

add_executable(sort_adjacencies app/sort_adjacencies.cpp $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libfiles>)
target_link_libraries(sort_adjacencies ${OpenMP_CXX_LIBRARIES})
install(TARGETS sort_adjacencies DESTINATION bin)

add_executable(online_mis app/online_mis.cpp $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libfiles> $<TARGET_OBJECTS:libonlinemis>)
target_compile_definitions(online_mis PRIVATE "-DOMIS")
target_link_libraries(online_mis ${OpenMP_CXX_LIBRARIES})
install(TARGETS online_mis DESTINATION bin)


