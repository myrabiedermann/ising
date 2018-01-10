set(CMAKE_CXX_COMPILER "g++-7")	# to avoid using clang on macbook
set(CMAKE_C_COMPILER "gcc-7")	# ,,


set(CMAKE_BUILD_TYPE Debug)

cmake_minimum_required(VERSION 3.0)
project(ising)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)  
find_package(Qt5Charts REQUIRED)

# The enhance functions
add_library(enhance SHARED lib/enhance.cpp)

include_directories("./gui/")
include_directories("./src/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fmax-errors=3 -Weffc++ -ftree-vectorize")
set(CMAKE_CXX_FLAGS_DEBUG          "-Og -g3 -ggdb -D_DEBUG -DDEBUG -DQT_NO_DEBUG")  # without qdebug
# set(CMAKE_CXX_FLAGS_DEBUG          "-Og -g3 -ggdb -DNDEBUG -DQT_DEBUG")     # with qdebug
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g1 -ggdb -DNDEBUG -DQT_NO_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -g0       -DNDEBUG -DQT_NO_DEBUG")


file(GLOB ising_SRC
  src/*.cpp
  src/*/*.cpp
  gui/*.cpp
  gui/*/*.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ~/bin)

# Create code from a list of Qt designer ui files.
set(CMAKE_AUTOUIC ON) # use this if you have CMake 3.x instead of the following
# qt5_wrap_ui(ising_SRC gui/ising.ui)

# Tell CMake to create the executable
add_executable(ising ${ising_SRC} ${sources})

# Use the Widgets module from Qt 5.
target_link_libraries(ising enhance Qt5::Widgets Qt5::Charts)

if(UNIX)
  install(FILES ${CMAKE_SOURCE_DIR}/ising.png DESTINATION /usr/share/pixmaps/ PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ WORLD_READ GROUP_READ)
  install(FILES ${CMAKE_SOURCE_DIR}/ising.desktop DESTINATION $ENV{HOME}/.local/share/applications/ PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ WORLD_READ GROUP_READ)
endif()
