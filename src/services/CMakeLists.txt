# Services library

set(SOURCES
    CounterService.cpp
    DocumentService.cpp
)

add_library(services STATIC ${SOURCES})

target_include_directories(services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
