add_library(SoundProcessor SoundProcessor.cpp SoundProcessor.h)

add_subdirectory(Config)
add_subdirectory(Converters)
add_subdirectory(WavHandler)

target_include_directories(SoundProcessor PRIVATE Config Converters WavHandler ${CMAKE_SOURCE_DIR})
target_link_libraries(SoundProcessor PRIVATE Config Converters WavHandler)

