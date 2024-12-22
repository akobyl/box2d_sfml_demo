cmake_minimum_required(VERSION 3.28)
project(box2d_smfl_demo LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
include(FetchContent)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(box2d)
#set_source_files_properties(${box2d_SOURCE_DIR}/src/*.cpp PROPERTIES LANGUAGE CXX) #For files ending in .cpp
#set_source_files_properties(${box2d_SOURCE_DIR}/src/*.c PROPERTIES LANGUAGE CXX) #For files ending in .c
#set_target_properties(box2d PROPERTIES CXX_STANDARD 17)
#set_target_properties(box2d PROPERTIES CXX_STANDARD_REQUIRED ON)
#set_target_properties(box2d PROPERTIES CXX_FLAGS "/EHsc")

add_executable(main main.cpp)
target_link_libraries(main PRIVATE
        sfml-graphics
        box2d
)
target_compile_features(main PRIVATE cxx_std_17)


if(WIN32)
    add_custom_command(
            TARGET main
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
            VERBATIM
    )
endif()