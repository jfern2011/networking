cmake_minimum_required(VERSION 3.14)

project(Networking VERSION 2.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

FetchContent_MakeAvailable(googletest)

# -----------------------------------------------------------------------------
# Create the shared library for this project
# -----------------------------------------------------------------------------
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
enable_testing()

add_executable(shared_fd-test
    tests/shared_fd-ut.cpp
    tests/main.cpp
)

target_link_libraries(shared_fd-test
PRIVATE
    networking
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(shared_fd-test)
