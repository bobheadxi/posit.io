cmake_minimum_required(VERSION 3.0)
project(posit)

# Flags and settings

set(CMAKE_CXX_STANDARD 17)

set(CXXFLAGS "-I/usr/local/include -std=c++1z -pthread")
set(LDFLAGS "-lstdc++ -L/usr/local/lib -lsodium")

set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LDFLAGS}")
set(EXECUTABLE_OUTPUT_PATH "bin")

# Source files

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/demo)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/googletest/)

set(COMMON_SOURCES
        posit.cpp
        include/netcode.io/netcode.c)

set(TEST_SOURCES
        posit_test.cpp)

set(DEMO_SOURCES
        demo.cpp
        demo/server.cpp
        demo/client.c)

# Make targets

add_custom_target(update
        COMMAND bash .scripts/setup.sh)

add_custom_target(docs
        COMMAND doxygen .doxygenrc)

add_executable(tests
        ${COMMON_SOURCES}
        ${TEST_SOURCES})
target_link_libraries(tests gtest_main)

add_executable(demo
        ${COMMON_SOURCES}
        ${DEMO_SOURCES})

add_custom_target(run_server
        COMMAND ./bin/demo server)

add_custom_target(run_client
        COMMAND ./bin/demo client)
