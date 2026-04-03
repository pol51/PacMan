cmake_minimum_required(VERSION 3.10)

project(PacMan)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

file(GLOB_RECURSE source_files src/*)

execute_process(
  COMMAND ../make_png.sh
  WORKING_DIRECTORY res/src)

file(GLOB_RECURSE resource_files ${CMAKE_CURRENT_SOURCE_DIR}/res/*.png)
file(GLOB_RECURSE data_files ${CMAKE_CURRENT_SOURCE_DIR}/data/*.txt)
file(GLOB_RECURSE resource_source_files ${CMAKE_CURRENT_SOURCE_DIR}/res/*.svg ${CMAKE_CURRENT_SOURCE_DIR}/res/*.sh)

file(COPY ${resource_files} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res)
file(COPY ${data_files} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

add_executable(PacMan ${source_files} ${data_files} ${resource_source_files})
target_link_libraries(PacMan SDL3::SDL3 SDL3_image::SDL3_image)