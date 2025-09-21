cmake_minimum_required(VERSION 3.10)
project(NativeFileDialogue LANGUAGES CXX)

# Set output directories (bin and obj dirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/NativeFileDialogue)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/NativeFileDialogue)

# Collect source files (recursive glob is discouraged but used here for brevity)
file(GLOB_RECURSE NFD_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

# Create a static library target
add_library(NativeFileDialogue STATIC ${NFD_SOURCES})

target_compile_features(NativeFileDialogue PRIVATE cxx_std_20)

# Configurations flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Kakali PRIVATE _DEBUG _CONSOLE)
    set_property(TARGET Kakali PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Kakali PRIVATE NDEBUG _CONSOLE)
    set_property(TARGET Kakali PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
elseif(CMAKE_BUILD_TYPE STREQUAL "Distribution")
    target_compile_definitions(Kakali PRIVATE DISTRIBUTION)
    set_property(TARGET Kakali PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()
