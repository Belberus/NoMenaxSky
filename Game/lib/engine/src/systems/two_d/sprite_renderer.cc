#include "engine/systems/two_d/sprite_renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/components/common/camera.h"
#include "engine/components/common/transform.h"
#include "engine/components/two_d/sprite.h"
#include "utils/shaders.h"

using namespace engine::systems::two_d;

struct SpriteRenderer::Impl {
  enum class Buffer { kVertices = 0, kIndices, kTexCoord };
  static const int kNumBuffers = 3;
  static GLuint program_;
  static GLuint vao_;
  static GLuint buffers_[kNumBuffers];
  static bool initialized_;

  static const GLchar *vert_shader_src;
  static const GLchar *frag_shader_src;
  static const glm::vec3 kVertices[4];
  static const GLuint kIndices[6];
};

SpriteRenderer::SpriteRenderer() : pimpl_(std::make_unique<Impl>()) {
  if (!pimpl_->initialized_) {
    pimpl_->initialized_ = true;
    GLuint vert =
        engine::utils::CompileShader(pimpl_->vert_shader_src, GL_VERTEX_SHADER);
    GLuint frag = engine::utils::CompileShader(pimpl_->frag_shader_src,
                                               GL_FRAGMENT_SHADER);
    pimpl_->program_ = engine::utils::LinkProgram(vert, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

    glGenVertexArrays(1, &pimpl_->vao_);
    glBindVertexArray(pimpl_->vao_);
    glGenBuffers(pimpl_->kNumBuffers, pimpl_->buffers_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 pimpl_->buffers_[static_cast<int>(Impl::Buffer::kIndices)]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pimpl_->kIndices),
                 pimpl_->kIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,
                 pimpl_->buffers_[static_cast<int>(Impl::Buffer::kVertices)]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pimpl_->kVertices), pimpl_->kVertices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,
                 pimpl_->buffers_[static_cast<int>(Impl::Buffer::kTexCoord)]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
  }
}

SpriteRenderer::~SpriteRenderer() {
  // glDeleteBuffers(kNumBuffers, buffers_);
  // glDeleteProgram(program_);
}

void SpriteRenderer::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  entityx::ComponentHandle<engine::components::common::Transform>
      camera_transform;
  entityx::ComponentHandle<engine::components::common::Camera>
      camera_description;
  glUseProgram(pimpl_->program_);
  glBindVertexArray(pimpl_->vao_);
  for (entityx::Entity cameras :
       es.entities_with_components(camera_transform, camera_description)) {
    entityx::ComponentHandle<engine::components::two_d::Sprite> graphics;
    entityx::ComponentHandle<engine::components::common::Transform> position;
    glUniformMatrix4fv(
        glGetUniformLocation(pimpl_->program_, "view"), 1, GL_FALSE,
        glm::value_ptr(camera_description->GetViewMatrix(*camera_transform)));
    glUniformMatrix4fv(
        glGetUniformLocation(pimpl_->program_, "projection"), 1, GL_FALSE,
        glm::value_ptr(camera_description->GetProjectionMatrix()));

    for (entityx::Entity entity :
         es.entities_with_components(graphics, position)) {
      glUniform4fv(glGetUniformLocation(pimpl_->program_, "in_color"), 1,
                   glm::value_ptr(graphics->GetColor()));
      auto texture = graphics->GetTexture();
      glActiveTexture(texture->texture_unit_id_);
      glBindTexture(GL_TEXTURE_2D, texture->texture_id_);
      glUniform1i(glGetUniformLocation(pimpl_->program_, "texture_sampler"),
                  texture->texture_unit_id_ - GL_TEXTURE0);
      glBindBuffer(GL_ARRAY_BUFFER,
                   pimpl_->buffers_[static_cast<int>(Impl::Buffer::kTexCoord)]);
      auto tex_coord = GetTexCoordVertices(graphics->GetTextureCoord());
      NormalizeTexCoordVertices(tex_coord, *texture);
      glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tex_coord.size(),
                   tex_coord.data(), GL_STATIC_DRAW);
      glm::mat4 model(1.0f);
      model = glm::scale(
          model, glm::vec3(graphics->GetTextureCoord().GetSize(), 1.0f));
      model = glm::translate(glm::mat4(), position->GetWorldPosition()) *
              glm::mat4_cast(position->GetWorldOrientation()) *
              glm::scale(glm::mat4(), position->GetWorldScale()) * model;
      glUniformMatrix4fv(glGetUniformLocation(pimpl_->program_, "model"), 1,
                         GL_FALSE, glm::value_ptr(model));
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
  }
  glBindVertexArray(0);
}

std::vector<glm::vec2> SpriteRenderer::GetTexCoordVertices(
    const engine::utils::Rectangle &rect) {
  std::vector<glm::vec2> tex_vertices;
  auto pos = rect.GetPosition();
  auto size = rect.GetSize();
  tex_vertices.push_back(pos);
  tex_vertices.push_back(pos + glm::vec2(size.x, 0));
  tex_vertices.push_back(pos + size);
  tex_vertices.push_back(pos + glm::vec2(0, size.y));
  return tex_vertices;
}

void SpriteRenderer::NormalizeTexCoordVertices(
    std::vector<glm::vec2> &coord,
    const engine::components::two_d::Texture &tex) {
  for (auto &c : coord) {
    c.x /= static_cast<float>(tex.GetWidth());
    c.y /= static_cast<float>(tex.GetHeight());
  }
}

bool SpriteRenderer::Impl::initialized_ = false;

GLuint SpriteRenderer::Impl::program_ = 0;

GLuint SpriteRenderer::Impl::vao_ = 0;

GLuint SpriteRenderer::Impl::buffers_[SpriteRenderer::Impl::kNumBuffers];

const GLchar *SpriteRenderer::Impl::vert_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec2 tex_coord;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform vec4 in_color;\n"
    "void main(void)\n"
    "{\n"
    "gl_Position = projection * view * model * vec4(position, 1.0);\n"
    "TexCoord = tex_coord;\n"
    "}\n";

const GLchar *SpriteRenderer::Impl::frag_shader_src =
    "#version 330 core\n"
    "out vec4 color;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D texture_sampler;\n"
    "uniform vec4 in_color;\n"
    "void main(void)\n"
    "{\n"
    " vec4 tex = texture(texture_sampler, TexCoord);\n"
    " color = tex + in_color;\n"
    "}\n";

const glm::vec3 SpriteRenderer::Impl::kVertices[4] = {
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f),
    glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(-0.5f, 0.5f, 0.0f)};

const GLuint SpriteRenderer::Impl::kIndices[6] = {0, 1, 2, 0, 3, 2};
