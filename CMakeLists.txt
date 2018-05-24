cmake_minimum_required(VERSION 3.0)
project(posit)

# Flags and settings

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z") # Travis workaround

set(CXXFLAGS "-I/usr/local/include")
set(LDFLAGS "-lstdc++ -L/usr/local/lib -lsodium")

set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LDFLAGS}")
set(EXECUTABLE_OUTPUT_PATH "bin")

# Source files

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/googletest/)

set(COMMON_SOURCES
        posit.cpp
        include/netcode.io/netcode.c)

set(TEST_SOURCES
        posit_test.cpp)

set(POC_SOURCES
        poc.cpp)

# Make targets

add_custom_target(update
        COMMAND bash .scripts/setup.sh)

add_executable(tests
        ${COMMON_SOURCES}
        ${TEST_SOURCES})
target_link_libraries(tests gtest_main)

add_executable(poc
        ${COMMON_SOURCES}
        ${POC_SOURCES})
