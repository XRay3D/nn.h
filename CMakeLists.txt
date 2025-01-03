cmake_minimum_required(VERSION 3.16)

project(NN LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

include_directories(../raylib/bin/include thirdparty)
link_directories(../raylib/bin/lib64)
add_compile_options(-msse3 -ffast-math -funroll-loops)
add_definitions(
  -DJetBrains="/usr/share/fonts/otf/jetbrains-mono/JetBrainsMono-Light.otf")

file(GLOB_RECURSE HPP *.hpp)

file(GLOB DEMOS cpp/*.cpp)
foreach(DEMO ${DEMOS})
  # break()
  get_filename_component(NAME ${DEMO} NAME_WE)
  add_executable(${NAME} ${DEMO} ${HPP})
  target_link_libraries(${NAME} PRIVATE raylib m)
  target_include_directories(${NAME} PRIVATE cpp)
endforeach()

# add_executable(NN adder.cpp ${HPP}) target_link_libraries(NN PRIVATE raylib m)
# include(GNUInstallDirs) install( TARGETS NN LIBRARY DESTINATION
# ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
