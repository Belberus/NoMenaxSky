cmake_minimum_required(VERSION 3.3)
project(engine)
set(CMAKE_CXX_STANDARD 14)

# IRRKLANG
if(UNIX)
  set(IRRKLANG_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/irrKlang/bin/linux-gcc-64/libIrrKlang.so)
elseif(WIN32)
  set(IRRKLANG_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/irrKlang/lib/Winx64-visualStudio/irrKlang.lib)
endif(UNIX)

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
set(ENTITYX_DT_TYPE float CACHE STRING "" FORCE)
add_subdirectory(lib/entityx)

# ASSIMP
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_COLLADA_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory(lib/assimp)
get_target_property(ASSIMP_INC assimp INCLUDE_DIRECTORIES)

# ENGINE
set(COMPONENTS_COMMON_HDR
  include/engine/components/common/camera.h
  include/engine/components/common/physics.h 
  include/engine/components/common/transform.h 
)
source_group(components\\common\\headers FILES ${COMPONENTS_COMMON_HDR})

set(COMPONENTS_COMMON_SRC
  src/components/common/camera.cc
  src/components/common/transform.cc
)
source_group(components\\common\\sources FILES ${COMPONENTS_COMMON_SRC})

set(COMPONENTS_TWO_D_HDR
  include/engine/components/two_d/aabb_collider.h
  include/engine/components/two_d/color_animation.h
  include/engine/components/two_d/sprite.h
  include/engine/components/two_d/sprite_animation.h
  include/engine/components/two_d/texture.h
  include/engine/components/two_d/tilemap.h
)
source_group(components\\two_d\\headers FILES ${COMPONENTS_TWO_D_HDR})

set(COMPONENTS_TWO_D_SRC
  src/components/two_d/sprite_animation.cc
  src/components/two_d/texture.cc
)
source_group(components\\two_d\\sources FILES ${COMPONENTS_TWO_D_SRC})

set(COMPONENTS_THREE_D_HDR
  include/engine/components/three_d/model.h
  src/components/three_d/model_impl.h
)
source_group(components\\three_d\\headers FILES ${COMPONENTS_THREE_D_HDR})

set(COMPONENTS_THREE_D_SRC
  src/components/three_d/model.cc
  src/components/three_d/model_impl.cc
)
source_group(components\\three_d\\sources FILES ${COMPONENTS_THREE_D_SRC})

set(SYSTEMS_TWO_D_HDR
  include/engine/systems/two_d/collider_renderer.h
  include/engine/systems/two_d/color_animator.h
  include/engine/systems/two_d/sprite_animator.h
  include/engine/systems/two_d/sprite_renderer.h
  include/engine/systems/two_d/physics.h
  include/engine/systems/two_d/tilemap_renderer.h
)
source_group(systems\\two_d\\headers FILES ${SYSTEMS_TWO_D_HDR})

set(SYSTEMS_TWO_D_SRC
  src/systems/two_d/collider_renderer.cc
  src/systems/two_d/color_animator.cc
  src/systems/two_d/sprite_animator.cc
  src/systems/two_d/sprite_renderer.cc
  src/systems/two_d/physics.cc
  src/systems/two_d/tilemap_renderer.cc
)
source_group(systems\\two_d\\sources FILES ${SYSTEMS_TWO_D_SRC})

set(SYSTEMS_THREE_D_HDR
  include/engine/systems/three_d/model_renderer.h
)
source_group(systems\\three_d\\headers FILES ${SYSTEMS_THREE_D_HDR})

set(SYSTEMS_THREE_D_SRC
  src/systems/three_d/model_renderer.cc
)
source_group(systems\\three_d\\sources FILES ${SYSTEMS_THREE_D_SRC})

set(EVENTS_HDR
  include/engine/events/collision.h
  include/engine/events/ignore_collision.h
  include/engine/events/key_pressed.h
  include/engine/events/key_released.h
)
source_group(events\\headers FILES ${EVENTS_HDR})

set(UTILS_HDR
  include/engine/utils/rectangle.h
  src/utils/shaders.h
)
source_group(utils\\headers FILES ${UTILS_HDR})

set(UTILS_SRC
  src/utils/shaders.cc
)
source_group(utils\\sources FILES ${UTILS_SRC})

set(CORE_HDR
  include/engine/core/audio_manager.h
  include/engine/core/engine.h
  include/engine/core/event_manager.h
  include/engine/core/input_manager.h
  include/engine/core/resource_manager.h
  include/engine/core/scene.h
  include/engine/core/scene_manager.h

)
source_group(core\\headers FILES ${CORE_HDR})

set(CORE_SRC
  src/core/audio_manager.cc
  src/core/engine.cc
  src/core/input_manager.cc 
  src/core/scene_manager.cc
)
source_group(core\\sources FILES ${CORE_SRC})

add_library(${PROJECT_NAME}
  ${COMPONENTS_COMMON_HDR}
  ${COMPONENTS_COMMON_SRC}
  ${COMPONENTS_TWO_D_HDR}
  ${COMPONENTS_TWO_D_SRC}
  ${COMPONENTS_THREE_D_HDR}
  ${COMPONENTS_THREE_D_SRC}
  ${SYSTEMS_TWO_D_HDR}
  ${SYSTEMS_TWO_D_SRC}
  ${SYSTEMS_THREE_D_HDR}
  ${SYSTEMS_THREE_D_SRC}
  ${EVENTS_HDR}
  ${UTILS_HDR}
  ${UTILS_SRC}
  ${CORE_HDR}
  ${CORE_SRC}
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    glm
    entityx
    glfw
  PRIVATE
	assimp
    gl3w
	${IRRKLANG_LIBS}
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    include
    lib/stb
    lib/irrKlang/include
  PRIVATE
    ${ASSIMP_INC}
	src
)
if(CMAKE_COMPILER_IS_GNUCC)
  target_compile_options(${PROJECT_NAME} 
	PRIVATE 
	  "-Wall"
  )
elseif(MSVC)
  target_compile_options(${PROJECT_NAME}
	PRIVATE
	  "/W4"
  )
endif(CMAKE_COMPILER_IS_GNUCC)
