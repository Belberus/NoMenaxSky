cmake_minimum_required(VERSION 3.3)
project(Game)

set(CMAKE_CXX_STANDARD 14)

# ENGINE
add_subdirectory(lib/engine)

#TMXLITE
set(TMXLITE_STATIC_LIB ON CACHE BOOL "" FORCE)
add_subdirectory(lib/tmxlite/tmxlite)

file(GLOB SOURCE_FILES src/* src/* lib/easybmp/*)
add_executable(${PROJECT_NAME}
  ${SOURCE_FILES}
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    engine
    tmxlite
)
target_include_directories(${PROJECT_NAME}
  PRIVATE
    src
    lib/easybmp
)

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

if(WIN32)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/engine/lib/irrKlang/bin/winx64-visualStudio/irrKlang.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )
endif(WIN32)
