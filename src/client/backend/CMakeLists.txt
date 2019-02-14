cmake_minimum_required(VERSION 3.0)   # CMake version check
project(datahandler)				  # Create project "datahandler"
set(CMAKE_CXX_STANDARD 11)            # require C++11 standard

add_executable(datahandler datahandler.cpp datahandler.hpp)
