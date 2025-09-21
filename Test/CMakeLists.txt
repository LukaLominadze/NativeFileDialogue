cmake_minimum_required(VERSION 3.10)
project(Test LANGUAGES CXX)

# Set output directories (bin and obj dirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/Test)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/Test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/Test)

# Collect source files (recursive glob is discouraged but used here for brevity)
file(GLOB_RECURSE TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_executable(Test ${TEST_SOURCES})

target_compile_features(Test PRIVATE cxx_std_20)

target_include_directories(Test PRIVATE
    ${CMAKE_SOURCE_DIR}/NativeFileDialogue/src
)

# Link dependencies
target_link_libraries(Test PRIVATE
    NativeFileDialogue
)

# Configurations flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Test PRIVATE _DEBUG _CONSOLE)
    set_property(TARGET Test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Test PRIVATE NDEBUG _CONSOLE)
    set_property(TARGET Test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
elseif(CMAKE_BUILD_TYPE STREQUAL "Distribution")
    target_compile_definitions(Test PRIVATE DISTRIBUTION)
    set_property(TARGET Test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()