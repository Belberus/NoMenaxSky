#include "engine/systems/two_d/tilemap_renderer.h"

#include <GL/gl3w.h>
#include <glm/gtc/type_ptr.hpp>

#include "engine/components/common/camera.h"
#include "engine/components/two_d/tilemap.h"
#include "utils/shaders.h"

struct engine::systems::two_d::TilemapRenderer::TilemapRenderer::Impl {
  Impl() : program_(0), vao_(0), vertices_buffer_(0) {}
  GLuint program_;
  GLuint vao_;
  GLuint vertices_buffer_;
  GLuint element_buffer_;
  GLuint tiles_buffer_;
  static const GLchar* vertex_shader_src_;
  static const GLchar* frag_shader_src_;
};

engine::systems::two_d::TilemapRenderer::TilemapRenderer()
    : pimpl_(std::make_unique<Impl>()) {
  GLuint vert = engine::utils::CompileShader(pimpl_->vertex_shader_src_,
                                             GL_VERTEX_SHADER);
  GLuint frag = engine::utils::CompileShader(pimpl_->frag_shader_src_,
                                             GL_FRAGMENT_SHADER);
  pimpl_->program_ = engine::utils::LinkProgram(vert, frag);
  glDeleteShader(vert);
  glDeleteShader(frag);

  float vertices[] = {0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0};
  unsigned int indices[] = {0, 1, 2, 2, 1, 3};
  glGenVertexArrays(1, &pimpl_->vao_);
  glBindVertexArray(pimpl_->vao_);
  glGenBuffers(1, &pimpl_->vertices_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, pimpl_->vertices_buffer_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &pimpl_->tiles_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, pimpl_->tiles_buffer_);
  glVertexAttribIPointer(1, 1, GL_INT, 0, (GLvoid*)0);
  glVertexAttribDivisor(1, 1);
  glEnableVertexAttribArray(1);
  glGenBuffers(1, &pimpl_->element_buffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pimpl_->element_buffer_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glBindVertexArray(0);
}

engine::systems::two_d::TilemapRenderer::~TilemapRenderer() {
  glDeleteProgram(pimpl_->program_);
}

void engine::systems::two_d::TilemapRenderer::update(
    entityx::EntityManager& es, entityx::EventManager& events,
    entityx::TimeDelta dt) {
  entityx::ComponentHandle<engine::components::common::Camera>
      camera_description;
  entityx::ComponentHandle<engine::components::common::Transform>
      camera_transform;
  glUseProgram(pimpl_->program_);
  for (auto camera :
       es.entities_with_components(camera_description, camera_transform)) {
    entityx::ComponentHandle<engine::components::two_d::Tilemap> tilemap;
    entityx::ComponentHandle<engine::components::common::Transform>
        tilemap_transform;
    for (auto e : es.entities_with_components(tilemap, tilemap_transform)) {
      auto texture = tilemap->texture_atlas_;
      glActiveTexture(texture->texture_unit_id_);
      glBindTexture(GL_TEXTURE_2D, texture->texture_id_);
      glUniform1i(glGetUniformLocation(pimpl_->program_, "texture_sampler"),
                  texture->texture_unit_id_ - GL_TEXTURE0);
      glBindVertexArray(pimpl_->vao_);
      glUniformMatrix4fv(glGetUniformLocation(pimpl_->program_, "model"), 1,
                         GL_FALSE,
                         glm::value_ptr(tilemap_transform->GetWorldMatrix()));
      glUniformMatrix4fv(
          glGetUniformLocation(pimpl_->program_, "view"), 1, GL_FALSE,
          glm::value_ptr(camera_description->GetViewMatrix(*camera_transform)));
      glUniformMatrix4fv(
          glGetUniformLocation(pimpl_->program_, "projection"), 1, GL_FALSE,
          glm::value_ptr(camera_description->GetProjectionMatrix()));
      glUniform1i(glGetUniformLocation(pimpl_->program_, "atlas_width"),
                  tilemap->texture_atlas_->GetWidth());
      glUniform1i(glGetUniformLocation(pimpl_->program_, "atlas_height"),
                  tilemap->texture_atlas_->GetHeight());
      glUniform1i(glGetUniformLocation(pimpl_->program_, "tile_size"),
                  tilemap->tile_size_);
      glUniform1i(
          glGetUniformLocation(pimpl_->program_, "num_tiles_horizontal"),
          tilemap->num_tiles_horizontal_);
      for (auto& layer : tilemap->layers_) {
        glBindBuffer(GL_ARRAY_BUFFER, pimpl_->tiles_buffer_);
        std::vector<int> tiles;
        for (auto& t : layer.tiles_) {
          tiles.push_back(t.id);
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * tiles.size(), tiles.data(),
                     GL_STATIC_DRAW);
        glDrawElementsInstanced(
            GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0,
            tilemap->num_tiles_horizontal_ * tilemap->num_tiles_vertical_);
      }
    }
  }
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  GLenum err(glGetError());

  while (err != GL_NO_ERROR) {
    std::string error;

    switch (err) {
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;
      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;
      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;
    }
    std::cerr << "GL_" << error.c_str() << std::endl;
    err = glGetError();
  }
}

const GLchar*
    engine::systems::two_d::TilemapRenderer::Impl::vertex_shader_src_ =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in int tile_num;\n"
        "out vec2 tex_coord;\n"
        "out float empty;\n"
        "out vec2 hey;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform int atlas_width;\n"
        "uniform int atlas_height;\n"
        "uniform int tile_size;\n"
        "uniform int num_tiles_horizontal;\n"
        "void main(void)\n"
        "{\n"
        "hey = vec2(float(tile_num), 0);\n"
        "vec2 tile_position = vec2(mod(gl_InstanceID, num_tiles_horizontal), "
        "floor(gl_InstanceID / num_tiles_horizontal));\n"
        "tile_position *= tile_size;\n"
        "vec3 final_position = vec3(tile_position, 0) + position * tile_size;\n"
        "gl_Position = projection * view * model * vec4(final_position, 1.0);\n"
        "vec2 num_tiles_atlas = vec2(float(atlas_width) / float(tile_size), "
        "float(atlas_height) / "
        "float(tile_size));\n"
        "vec2 tile_position_atlas = vec2(mod((tile_num - 1), "
        "num_tiles_atlas.x), "
        "floor((tile_num - 1.0) / num_tiles_atlas.x));\n"
        "tile_position_atlas *= tile_size;\n"
        "tile_position_atlas += position.xy * tile_size;\n"
        "tex_coord = vec2(float(tile_position_atlas.x) / float(atlas_width), "
        "float(tile_position_atlas.y) / float(atlas_height));\n"
        "empty = tile_num == 0 ? 1.0 : 0.0;\n"
        "}\n";

const GLchar* engine::systems::two_d::TilemapRenderer::Impl::frag_shader_src_ =
    "#version 330 core\n"
    "out vec4 color;\n"
    "in vec2 tex_coord;\n"
    "in float empty;\n"
    "in vec2 hey;\n"
    "uniform sampler2D texture_sampler;\n"
    "void main(void)\n"
    "{\n"
    "if (empty > 0.4) {\n"
    "discard;\n"
    "}\n"
    "color = texture(texture_sampler, tex_coord);\n"
    "}\n";
