cmake_minimum_required(VERSION 3.20)

if(NOT VCPKG_ROOT)
    set(VCPKG_ROOT $ENV{VCPKG_ROOT})
    if(NOT VCPKG_ROOT)
        message(FATAL_ERROR "VCPKG_ROOT environment variable is not set. Please set it to the root of your vcpkg installation.")
    endif()
endif()

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(lariat-data-platform)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(spdlog CONFIG REQUIRED)
find_package(yyjson CONFIG REQUIRED)

add_library(ldpcore INTERFACE)
target_include_directories(ldpcore INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB LDPX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(ldpx ${LDPX_SOURCES})
target_include_directories(ldpx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(ldpx PRIVATE ldpcore)