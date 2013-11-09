cmake_minimum_required(VERSION 2.6)

project(PacMan)

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)

include_directories(${SDL_INCLUDE_DIR} src)

link_libraries (
  ${SDL_LIBRARY}
  ${SDLIMAGE_LIBRARY}
  SDLmain
)

file(GLOB_RECURSE source_files src/*)

execute_process(
  COMMAND ../make_png.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res/src)

file(GLOB_RECURSE resource_files res/*.png)

execute_process(
  COMMAND mkdir -p res
  COMMAND cp -rf ${resource_files} res)


add_executable(PacMan ${source_files})
