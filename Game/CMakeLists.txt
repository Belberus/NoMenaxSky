cmake_minimum_required (VERSION 3.3)
project (Game)
set (CMAKE_CXX_STANDARD 11)
cmake_policy(SET CMP0024 OLD)

# IRRKLANG
if (UNIX)
    link_directories(lib/irrKlang/bin/linux-gcc-64)
    set(IRRKLANG_LIBS ikpFlac.so ikpMP3.so libIrrKlang.so)
endif (UNIX)
if (WIN32)
    link_directories(lib/irrKlang/lib/Winx64-visualStudio)
    set(IRRKLANG_LIBS irrKlang.lib)
endif (WIN32)
include_directories(lib/irrKlang/include)

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

include_directories(lib/stb)


file(GLOB_RECURSE SOURCE_FILES src/*)
set_source_files_properties(${GL3W_SOURCES} PROPERTIES GENERATED TRUE)
set(SOURCE_FILES ${SOURCE_FILES} ${GL3W_SOURCES})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} gl3w)
target_link_libraries(${PROJECT_NAME} ${GL3W_LIB} glfw ${GLFW_LIBRARIES} entityx ${IRRKLANG_LIBS})

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/irrKlang/bin/winx64-visualStudio/irrKlang.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif (WIN32)
