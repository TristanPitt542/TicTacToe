cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

# ====================================================================
# AUTOMATIC FILE DETECTION
# ====================================================================
# This searches your folders and grabs every source/header file it finds
file(GLOB_RECURSE AUTOMATIC_SRC_FILES 
    "src/*.cpp"
    "src/*.hpp"
    "Include/*.hpp"
)

# Pass the automated list to your executable target
add_executable(main ${AUTOMATIC_SRC_FILES})
# ====================================================================

# Tells the compiler where to look for local headers
target_include_directories(main PRIVATE Include Include/Game)

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)