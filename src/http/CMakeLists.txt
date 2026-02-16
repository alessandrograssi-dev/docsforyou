# HTTP Server library

set(SOURCES
    Server.cpp
    HttpConnection.cpp
)

add_library(http_server STATIC ${SOURCES})

target_include_directories(http_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(http_server PUBLIC ${Boost_LIBRARIES})
