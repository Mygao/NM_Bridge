cmake_minimum_required (VERSION 2.6)
project (NM_Bridge)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

LIST(APPEND ${PROJECT_NAME}_SRCS
   include/front_server.h
   include/rsi_server.h
   include/simple_xml.h
   src/front_server.cpp
   src/rsi_server.cpp
   src/simple_xml.cpp
   main.cpp)

# ----------------------------------------------------------------------------
#  boost include/library directories:
# ----------------------------------------------------------------------------
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost COMPONENTS system thread regex REQUIRED)

include_directories(include)
include_directories(/usr/local/include)

IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

#find_package(ZMQ REQUIRED)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})

target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})
