cmake_minimum_required (VERSION 2.6)
project (NM_Bridge)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost COMPONENTS system thread REQUIRED)

include_directories(/usr/local/include)

IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

#find_package(ZMQ REQUIRED)

add_executable(NM_Bridge main.cpp src/rsi_server.cpp src/simple_xml.cpp src/front_server.cpp)

target_link_libraries (NM_Bridge ${Boost_LIBRARIES})
