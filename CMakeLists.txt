cmake_minimum_required(VERSION 3.15)
project(docsforyou CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Boost from vendors directory (header-only)
set(BOOST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendors/boost)
include_directories(${BOOST_INCLUDE_DIR})

# Define Boost_LIBRARIES as empty since we're using header-only parts
set(Boost_LIBRARIES "")

# Add subdirectories
add_subdirectory(src/http)

# Main executable
add_executable(docsforyou src/main.cpp)
target_link_libraries(docsforyou PRIVATE http_server ${Boost_LIBRARIES})
