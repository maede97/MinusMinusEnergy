cmake_minimum_required(VERSION 3.1)   # CMake version check
project(datahandler)				  # Create project "datahandler"
set(CMAKE_CXX_STANDARD 11)            # require C++11 standard

# compile data handler
add_executable(datahandler datahandler.cpp datahandler.hpp)
target_link_libraries(datahandler sqlite3)
target_compile_options(datahandler PUBLIC -O2)
