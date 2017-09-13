
# cmake_minimum_required(VERSION 2.8.11)
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

find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include_directories("./gui/")
include_directories("./src/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fmax-errors=3 -Weffc++ -ftree-vectorize")
set(CMAKE_CXX_FLAGS_DEBUG          "-Og -g3 -ggdb -D_DEBUG -DDEBUG -DQT_DEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g1 -ggdb -DNDEBUG -DQT_NO_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -g0       -DNDEBUG -DQT_NO_DEBUG")

set(ising_SRC
  src/main.cpp
  src/ising.cpp
)

file(GLOB ising_SRC
  src/*.cpp
  src/*/*.cpp
  gui/*.cpp
)

# Create code from a list of Qt designer ui files.
set(CMAKE_AUTOUIC ON) # use this if you have CMake 3.x instead of the following
# qt5_wrap_ui(ising_SRC gui/ising.ui)

# Tell CMake to create the helloworld executable
add_executable(ising ${ising_SRC} ${sources})


set(CMAKE_EXE_LINKER_FLAGS "${STDCXX_LDFLAGS} -lboost_program_options -lboost_system -lboost_filesystem")


# Use the Widgets module from Qt 5.
target_link_libraries(ising Qt5::Widgets Qt5::Charts)

# Install the executable
install(TARGETS ising DESTINATION bin)

