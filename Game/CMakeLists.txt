cmake_minimum_required(VERSION 3.3)
project(Game)
set(CMAKE_CXX_STANDARD 11)

# IRRKLANG
if(UNIX)
  link_directories(lib/irrKlang/bin/linux-gcc-64)
  set(IRRKLANG_LIBS libIrrKlang.so)
elseif(WIN32)
  link_directories(lib/irrKlang/lib/Winx64-visualStudio)
  set(IRRKLANG_LIBS irrKlang.lib)
endif(UNIX)
include_directories(lib/irrKlang/include)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

# GLM
add_subdirectory(lib/glm)

# GL3W
add_subdirectory(lib/gl3w)
get_property(GL3W_SOURCES TARGET gl3w PROPERTY INTERFACE_SOURCES)
set_source_files_properties(${GL3W_SOURCES} PROPERTIES GENERATED TRUE)

# ENTITYX
set(ENTITYX_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ENTITYX_BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/entityx)

# STB
include_directories(lib/stb)

file(GLOB_RECURSE SOURCE_FILES src/*)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${IRRKLANG_LIBS} gl3w glfw glm entityx)
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
if(WIN32)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/irrKlang/bin/winx64-visualStudio/irrKlang.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif(WIN32)
