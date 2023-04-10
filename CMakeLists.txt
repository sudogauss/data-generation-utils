cmake_minimum_required(VERSION 3.5...3.16)

file(READ VERSION PROJECT_VERSION)
project(dgu VERSION "${VERSION}" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_library(dgu INTERFACE)

target_include_directories(dgu INTERFACE include)

find_package(Boost REQUIRED)

target_link_libraries(
    dgu
    INTERFACE
    Boost::random
    Boost::iterator
)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    
    option(DGU_BUILD_EXAMPLES   "whether or not examples should be built" ON)
    option(DGU_BUILD_TESTS      "whether or not tests should be built" ON)

    if(DGU_BUILD_EXAMPLES)
        add_subdirectory(examples)
    endif()
    if(DGU_BUILD_TESTS)
        # enable_testing()
        add_subdirectory(tests)
    endif()

endif()