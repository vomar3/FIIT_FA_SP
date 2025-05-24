cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

project(mp_os LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DCROW_GENERATE_CMAKE ON)

set(VCPKG_TARGET_TRIPLET x64-linux)
set(CMAKE_TOOLCHAIN_FILE /home/max/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(DVCPKG_TARGET_TRIPLET x64-linux)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

find_package(Boost COMPONENTS system container REQUIRED) # Ставить через vcpkg

find_package(Crow REQUIRED)

add_subdirectory(allocator)
add_subdirectory(arithmetic)
add_subdirectory(associative_container)
add_subdirectory(common)
add_subdirectory(logger)
add_compile_options(-std=c++23)