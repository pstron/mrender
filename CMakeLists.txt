cmake_minimum_required(VERSION 3.30)
project(mrender VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++ -lc++abi)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

set(STD_PCM ${CMAKE_BINARY_DIR}/modules/std.pcm)
add_custom_command(
    OUTPUT ${STD_PCM}
    COMMAND clang++ -std=c++23 -stdlib=libc++ --precompile 
            -o ${STD_PCM} /usr/share/libc++/v1/std.cppm
    VERBATIM
)
add_custom_target(std_module ALL DEPENDS ${STD_PCM})

file(GLOB_RECURSE MODULE_SOURCES src/*.cppm)
if(MODULE_SOURCES)
    add_library(project_modules)
    target_sources(project_modules PUBLIC FILE_SET CXX_MODULES FILES ${MODULE_SOURCES})
    target_compile_options(project_modules PUBLIC 
        -fmodule-file=std=${STD_PCM}
        -fprebuilt-module-path=${CMAKE_BINARY_DIR}/modules
    )
    add_dependencies(project_modules std_module)
endif()

file(GLOB_RECURSE CPP_SOURCES src/*.cpp)
if(NOT CPP_SOURCES)
    message(FATAL_ERROR "No .cpp files found in src/")
endif()

add_executable(main ${CPP_SOURCES})
target_compile_options(main PRIVATE 
    -fmodule-file=std=${STD_PCM}
    -fprebuilt-module-path=${CMAKE_BINARY_DIR}/modules
)
add_dependencies(main std_module)

if(MODULE_SOURCES)
    target_link_libraries(main PRIVATE project_modules)
endif()

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
