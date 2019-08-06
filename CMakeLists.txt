cmake_minimum_required(VERSION 3.7)
project(ognDataLogger)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cpp-lib/cmake/")

find_package(ogn_lib REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(png++ REQUIRED)
find_package(Threads REQUIRED)
find_package(dkulpaclibs REQUIRED)
find_package(CURL REQUIRED)

include_directories(${png++_INCLUDE_DIR})

add_executable(ognDataLogger main.cpp LogPusher.cpp LogPusher.h OgnLogger.cpp OgnLogger.h DebugLog.cpp DebugLog.h)
target_link_libraries(ognDataLogger ${ogn_lib_LIB} Eigen3::Eigen ${CMAKE_THREAD_LIBS_INIT} ${dkulpaclibs_misc_LIB} ${CURL_LIBRARIES})