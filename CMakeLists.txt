cmake_minimum_required(VERSION 3.22)

project(pi-test-cpp)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    cmake-require-content
    GIT_REPOSITORY "git@github.com:weagle08/cmake-require-content.git"
    GIT_TAG "main"
)
FetchContent_MakeAvailable(cmake-require-content)

include(RequireContent)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(STATUS "****ADDING DEBUG COMPILE DEFINITION****")
    add_compile_definitions(DEBUG)
endif()

# add_compile_options()
include(AddExecutable)

executable(
    ${CMAKE_PROJECT_NAME}
    "src/main.cpp"
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    libgpiod.a
)
