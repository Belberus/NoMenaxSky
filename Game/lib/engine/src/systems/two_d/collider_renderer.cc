#include "engine/systems/two_d/collider_renderer.h"

#include <GL/gl3w.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/components/common/camera.h"
#include "engine/components/common/transform.h"
#include "engine/components/two_d/aabb_collider.h"
#include "utils/shaders.h"

struct engine::systems::two_d::ColliderRenderer::Impl {
  GLuint vao_;
  GLuint buffer_;
  GLuint program_;
  static const GLchar *vertex_shader_src_;
  static const GLchar *frag_shader_src_;
};

engine::systems::two_d::ColliderRenderer::ColliderRenderer()
    : pimpl_(std::make_unique<Impl>()) {
  GLuint vert = engine::utils::CompileShader(pimpl_->vertex_shader_src_,
                                             GL_VERTEX_SHADER);
  GLuint frag = engine::utils::CompileShader(pimpl_->frag_shader_src_,
                                             GL_FRAGMENT_SHADER);
  pimpl_->program_ = engine::utils::LinkProgram(vert, frag);
  glDeleteShader(vert);
  glDeleteShader(frag);
  glGenVertexArrays(1, &pimpl_->vao_);
  glBindVertexArray(pimpl_->vao_);
  glGenBuffers(1, &pimpl_->buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, pimpl_->buffer_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

engine::systems::two_d::ColliderRenderer::~ColliderRenderer() {
  glDeleteProgram(pimpl_->program_);
  glDeleteBuffers(1, &pimpl_->buffer_);
  glDeleteVertexArrays(1, &pimpl_->vao_);
}

void engine::systems::two_d::ColliderRenderer::update(
    entityx::EntityManager &es, entityx::EventManager &events,
    entityx::TimeDelta dt) {
  entityx::ComponentHandle<engine::components::common::Transform>
      camera_transform;
  entityx::ComponentHandle<engine::components::common::Camera>
      camera_description;
  for (entityx::Entity cameras :
       es.entities_with_components(camera_transform, camera_description)) {
    entityx::ComponentHandle<engine::components::two_d::AABBCollider> collider;
    entityx::ComponentHandle<engine::components::common::Transform> transform;
    for (entityx::Entity entity :
         es.entities_with_components(collider, transform)) {
      auto bl = collider->center - collider->half_size;
      auto br = collider->center +
                glm::vec2(collider->half_size.x, -1.0f * collider->half_size.y);
      auto tl = collider->center +
                glm::vec2(-1.0f * collider->half_size.x, collider->half_size.y);
      auto tr = collider->center + collider->half_size;
      glm::vec3 data[] = {glm::vec3(bl, 0.0f), glm::vec3(br, 0.0f),
                          glm::vec3(tr, 0.0f), glm::vec3(tl, 0.0f)};
      glBindVertexArray(pimpl_->vao_);
      glBindBuffer(GL_ARRAY_BUFFER, pimpl_->buffer_);
      glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
      glUseProgram(pimpl_->program_);
      glUniformMatrix4fv(glGetUniformLocation(pimpl_->program_, "model"), 1,
                         GL_FALSE, glm::value_ptr(transform->GetWorldMatrix()));
      glUniformMatrix4fv(
          glGetUniformLocation(pimpl_->program_, "view"), 1, GL_FALSE,
          glm::value_ptr(camera_description->GetViewMatrix(*camera_transform)));
      glUniformMatrix4fv(
          glGetUniformLocation(pimpl_->program_, "projection"), 1, GL_FALSE,
          glm::value_ptr(camera_description->GetProjectionMatrix()));
      glDrawArrays(GL_LINE_LOOP, 0, 4);
      glBindVertexArray(0);
    }
  }
}

const GLchar
    *engine::systems::two_d::ColliderRenderer::Impl::vertex_shader_src_ =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "void main(void)\n"
        "{\n"
        "gl_Position = projection * view * model * vec4(position, 1.0);\n"
        "}\n";

const GLchar *engine::systems::two_d::ColliderRenderer::Impl::frag_shader_src_ =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main(void)\n"
    "{\n"
    " color = vec4(1.0, 0.0, 1.0, 1.0);\n"
    "}\n";
