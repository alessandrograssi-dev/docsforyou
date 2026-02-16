cmake_minimum_required(VERSION 3.15)

# Set policy for Boost module
cmake_policy(SET CMP0167 OLD)

project(docsforyou CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find system Boost libraries
find_package(Boost 1.78 REQUIRED COMPONENTS system filesystem)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found. Install with: sudo apt install libboost-all-dev")
endif()

message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

include_directories(${Boost_INCLUDE_DIRS})

# Add subdirectories
add_subdirectory(src/http)
add_subdirectory(src/services)
add_subdirectory(src/repository)

set(SOURCES
    src/main.cpp
)

set(LIBRARIES
    http_server
    services
    repository
)

# Main executable
add_executable(docsforyou ${SOURCES})
target_link_libraries(docsforyou PRIVATE ${LIBRARIES} ${Boost_LIBRARIES})
