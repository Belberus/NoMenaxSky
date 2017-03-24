cmake_minimum_required (VERSION 3.3)
project (Game)
set (CMAKE_CXX_STANDARD 11)
cmake_policy(SET CMP0024 OLD)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)

# GLM
include_directories(lib/glm)

# GL3W
add_subdirectory(lib/gl3w)
find_package(gl3w REQUIRED)
get_property(GL3W_SOURCES TARGET gl3w PROPERTY INTERFACE_SOURCES)
get_property(GL3W_INCLUDE TARGET gl3w PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
get_property(GL3W_LIB TARGET gl3w PROPERTY INTERFACE_LINK_LIBRARIES)
include_directories(${GL3W_INCLUDE})

# ENTITYX
set(ENTITYX_BUILD_SHARED false CACHE BOOL "")
set(ENTITYX_BUILD_TESTING false CACHE BOOL "")
add_subdirectory(lib/entityx)
# hackish
include_directories(lib/entityx/)
link_directories("${CMAKE_CURRENT_BINARY_DIR}/lib/entityx/")

# FREEIMAGE
find_library(FREE_IMAGE NAMES freeimage FreeImage)
if (FREE_IMAGE)
    include_directories(${FREE_IMAGE_INCLUDE_DIRS})
    file(GLOB_RECURSE SOURCE_FILES src/*)
    set_source_files_properties(${GL3W_SOURCES} PROPERTIES GENERATED TRUE)
    set(SOURCE_FILES ${SOURCE_FILES} ${GL3W_SOURCES})
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    add_dependencies(${PROJECT_NAME} gl3w)
    target_link_libraries(${PROJECT_NAME} ${GL3W_LIB} ${FREE_IMAGE} glfw ${GLFW_LIBRARIES} entityx)
else ()
    message(FATAL_ERROR "Install FreeImage library")
endif (FREE_IMAGE)
