cmake_minimum_required(VERSION 3.0)
project(posit)

# Flags and settings

set(CMAKE_CXX_STANDARD 14)
set(CXXFLAGS "-I/usr/local/include")
set(LDFLAGS "-lstdc++ -L/usr/local/lib -lsodium")

set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LDFLAGS}")

# Source files

include_directories(${PROJECT_SOURCE_DIR}/vendor)

set(COMMON_SOURCES
        posit.cpp
        vendor/netcode.io/netcode.c)

set(POC_SOURCES
        poc.cpp)

# Make targets

add_custom_target(setup 
        COMMAND bash .scripts/setup.sh)

add_executable(poc
        ${COMMON_SOURCES}
        ${POC_SOURCES})
