project(readWriteData)
cmake_minimum_required(VERSION 2.8)


#################### Enable c++14/c++11 ##############################
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1y)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX1y)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
elseif(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++11/C++14 support.")
endif()
######################################################################

####### Switch between Debug and Release mode ################
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
else()
    message("Release mode")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
##################################################################

############ Include OpenCV library ########################
find_package( OpenCV REQUIRED ) # Simple way

#find_package( OpenCV REQUIRED ) # Complex way
#if( OpenCV_FOUND )
#list( APPEND ThirdParty_LIBS ${OpenCV_LIBS} )
#    include_directories( ${OpenCV_INCLUDE_DIRS} )
#endif( OpenCV_FOUND )
###############################################################

FILE(GLOB_RECURSE INC_ALL "*.jpg" "*.md" "*.png") # specify files to be included in the project list

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${INC_ALL})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS}) # link against the libraries

