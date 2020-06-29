cmake_minimum_required(VERSION 3.10)

project(PacMan)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} src)

file(GLOB_RECURSE source_files src/*)

execute_process(
  COMMAND ../make_png.sh
  WORKING_DIRECTORY res/src)

file(GLOB_RECURSE resource_files res/*.png)

execute_process(
  COMMAND mkdir -p res
  COMMAND cp -f ${resource_files} res)

file(GLOB_RECURSE data_files data/*.txt)

file(GLOB_RECURSE resource_source_files res/*.svg res/*.sh)

execute_process(
  COMMAND mkdir -p data
  COMMAND cp -f ${data_files} data)

add_executable(PacMan ${source_files} ${data_files} ${resource_source_files})
target_link_libraries(PacMan SDL2::Main SDL2::Image)