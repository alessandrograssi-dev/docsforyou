# Repository library

set(SOURCES
    FileDocumentRepository.cpp
)

add_library(repository STATIC ${SOURCES})

target_include_directories(repository PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
