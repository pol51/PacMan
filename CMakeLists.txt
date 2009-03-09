CMake_Minimum_Required(VERSION 2.6)

Project(sdl_test)
Set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

Find_Package(SDL REQUIRED)
Find_Package(SDL_image REQUIRED)

Include_Directories(${SDL_INCLUDE_DIR} src)

Link_Libraries (
  ${SDL_LIBRARY}
  ${SDLIMAGE_LIBRARY}
  SDLmain
)

File(GLOB_RECURSE source_files src/*)

Add_Executable(sdl_test ${source_files})
