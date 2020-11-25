set(SOURCES include/logger/Logger.hpp Logger.cpp Logger_internal.hpp)
add_library(logger ${SOURCES})
target_include_directories(logger
    PUBLIC include
    PRIVATE .
    )
target_link_libraries(logger fmt)

