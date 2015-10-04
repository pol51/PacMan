cmake_minimum_required(VERSION 2.8)

project(PacMan)

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)

include_directories(${SDL_INCLUDE_DIR} src)

add_definitions(-std=c++11)

link_libraries (
  ${SDL_LIBRARY}
  ${SDLIMAGE_LIBRARY}
  SDLmain
)

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
