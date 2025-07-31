cmake_minimum_required(VERSION 3.11)
project(CubeShading)


add_executable(${PROJECT_NAME} ./CubeShading/main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -O2)
if(UNIX)
    set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.6.2/lib/cmake/SFML)
endif(UNIX)
if(WIN32)
    set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.6.1/lib/cmake/SFML)
endif(WIN32)

find_package(SFML COMPONENTS window graphics system REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ./SFML-2.6.1/include)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-graphics sfml-window)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )
