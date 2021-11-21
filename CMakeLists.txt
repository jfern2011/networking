cmake_minimum_required(VERSION 3.14)

project(Networking VERSION 2.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/googletest/googletest
                 EXCLUDE_FROM_ALL)

add_library(networking STATIC
    src/file_descriptor.cpp
    src/shared_fd.cpp
    src/unique_fd.cpp
)

target_include_directories(networking
PUBLIC
    include
)

# -----------------------------------------------------------------------------
# Build unit tests
# -----------------------------------------------------------------------------
add_executable(shared_fd-test
    tests/shared_fd-ut.cpp
    tests/main.cpp
)

#target_include_directories(abort-test
#    PRIVATE
#    ${CMAKE_CURRENT_LIST_DIR}/googletest/googletest/include
#)

target_link_libraries(shared_fd-test
PRIVATE
    networking
    gtest
)
