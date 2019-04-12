# ===================================================================================
#  dkulpaclibs CMake configuration file
#
#             ** File generated automatically, do not modify **
#
#  Usage from an external project:
#    In your CMakeLists.txt, add these lines:
#       find_package(dkulpaclibs)
#       
#
#    FIND_PACKAGE(dkulpaclibs REQUIRED)
#    TARGET_LINK_LIBRARIES(MY_TARGET_NAME dkulpaclibs_misc_LIB dkulpaclibs_hw_LIB)
#
#
# ===================================================================================

find_library(ogn_lib_LIB
        NAMES cpp-lib
        PATHS "${CMAKE_CURRENT_SOURCE_DIR}/cpp-lib/"
        PATH_SUFFIXES obj/opt)

find_path(ogn_lib_INC
        NAMES cpp-lib/
        PATHS "${CMAKE_CURRENT_SOURCE_DIR}/cpp-lib/"
        PATH_SUFFIXES include)

message("OGN Lib:")
message(${ogn_lib_INC})
message(${ogn_lib_LIB})

include_directories(${ogn_lib_INC})
