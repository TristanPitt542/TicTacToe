cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

# Output compiled binary to a clean bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ====================================================================
# FETCH SFML 3.1.0 DIRECTLY FROM GITHUB
# ====================================================================
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

# Scan for .cpp and .hpp
file(GLOB_RECURSE AUTOMATIC_FILES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.hpp"
    "Include/*.hpp"
)

# Automated List
add_executable(main ${AUTOMATIC_FILES})

target_include_directories(main PRIVATE 
    Include 
    Include/Game
    Include/Game/UI
    Include/Game/UI/Menus
    src
    src/UI
)

target_compile_features(main PRIVATE cxx_std_20)
target_link_libraries(main PRIVATE 
    SFML::Graphics
    SFML::Audio
)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
    "$<TARGET_FILE_DIR:main>/Assets"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)