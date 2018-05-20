cmake_minimum_required(VERSION 3.0)
project(posit)

# Flags and settings

set(CMAKE_CXX_STANDARD 14)
set(COMPILE_FLAGS "-I/usr/local/include")
set(LINKER_FLAGS "-lstdc++ -L/usr/local/lib -lsodium")

# Source files

include_directories(${PROJECT_SOURCE_DIR}/vendor)

set(COMMON_SOURCES
        posit.cpp
        vendor/netcode.io/netcode.c)

set(POC_SOURCES
        poc.cpp)

# Make targets

install(CODE "execute_process(COMMAND .scripts/install.sh)")

add_executable(poc
        ${COMMON_SOURCES}
        ${POC_SOURCES})
