cmake_minimum_required(VERSION 3.13)
project(ognDataLogger)

set(CMAKE_CXX_STANDARD 14)

find_package(ogn_lib REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(png++ REQUIRED)
find_package(Threads REQUIRED)
find_package(dkulpaclibs REQUIRED)

include_directories(${png++_INCLUDE_DIR})

add_executable(ognDataLogger main.cpp LogPusher.cpp LogPusher.h OgnLogger.cpp OgnLogger.h)
target_link_libraries(ognDataLogger ${ogn_lib_LIB} Eigen3::Eigen ${CMAKE_THREAD_LIBS_INIT} ${dkulpaclibs_misc_LIB})