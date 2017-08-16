# SET( CMAKE_BUILD_TYPE Release )
# SET( CMAKE_BUILD_TYPE Relwithdebinfo )
SET( CMAKE_BUILD_TYPE Debug )

set(CMAKE_CXX_COMPILER "g++-7")
cmake_minimum_required(VERSION 2.8)
project(ising)


find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})


execute_process(COMMAND ${CMAKE_CXX_COMPILER} -v)
set(CMAKE_CXX_FLAGS  "-std=c++14 ${CMAKE_CXX_FLAGS} -Wall -Wpedantic -fmax-errors=3 -frounding-math -fsignaling-nans " )
SET(CMAKE_CXX_FLAGS_DEBUG          "-Og -g3 -ggdb -DDEBUG -D_DEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g1 -ggdb -DNDEBUG -DEIGEN_NO_DEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE        "-O3 -g0       -DNDEBUG -DEIGEN_NO_DEBUG")
set( ${CMAKE_EXE_LINKER_FLAGS} "${STDCXX_LDFLAGS} -lboost_program_options -lboost_system -lboost_filesystem ")


file( GLOB sources
  src/*.cpp
  src/input/*.cpp
  src/system/*.cpp
  src/utility/*.cpp
)
add_executable(ising ${sources})


target_link_libraries(ising  ${STDCXX_LDFLAGS} ${Boost_LIBRARIES})


install(TARGETS ising RUNTIME DESTINATION bin)


if( CMAKE_BUILD_TYPE MATCHES "Debug" )
    message( WARNING "BUILD TYPE: ${CMAKE_BUILD_TYPE}")
else()
    message( STATUS "BUILD TYPE: ${CMAKE_BUILD_TYPE}")
endif()
