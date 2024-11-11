cmake_minimum_required(VERSION 3.22)
project(Lab3 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(Lab3 Main.cpp CommandStructure.h)

add_subdirectory(SoundProcessor)

target_include_directories(Lab3 PRIVATE . SoundProcessor Config Converters WavHandler)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(Lab3 PRIVATE SoundProcessor Config Converters WavHandler gtest_main)