#include <GL/gl3w.h>

#include "engine/systems/three_d/model_renderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "components/three_d/model_impl.h"
#include "engine/components/common/camera.h"
#include "engine/components/common/transform.h"
#include "engine/components/three_d/model.h"
#include "utils/shaders.h"

struct engine::systems::three_d::ModelRenderer::Impl {
  static const GLchar *vert_shader_src;
  static const GLchar *frag_shader_src;
  GLuint program;
};

const GLchar *engine::systems::three_d::ModelRenderer::Impl::vert_shader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 normal;"
    "layout (location = 2) in vec2 tex_coord;\n"
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

const GLchar *engine::systems::three_d::ModelRenderer::Impl::frag_shader_src =
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

engine::systems::three_d::ModelRenderer::ModelRenderer() : pimpl_(new Impl()) {
  GLuint vert =
      engine::utils::CompileShader(pimpl_->vert_shader_src, GL_VERTEX_SHADER);
  GLuint frag =
      engine::utils::CompileShader(pimpl_->frag_shader_src, GL_FRAGMENT_SHADER);
  pimpl_->program = engine::utils::LinkProgram(vert, frag);
  glDeleteShader(vert);
  glDeleteShader(frag);
}

void engine::systems::three_d::ModelRenderer::update(
    entityx::EntityManager &es, entityx::EventManager &events,
    entityx::TimeDelta dt) {
  entityx::ComponentHandle<engine::components::common::Transform>
      camera_transform;
  entityx::ComponentHandle<engine::components::common::Camera> camera_info;
  glUseProgram(pimpl_->program);
  glUniform4fv(glGetUniformLocation(pimpl_->program, "in_color"), 1,
               glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));
  for (entityx::Entity camera :
       es.entities_with_components(camera_transform, camera_info)) {
    entityx::ComponentHandle<engine::components::common::Transform>
        model_transform;
    entityx::ComponentHandle<engine::components::three_d::Model> model_info;
    glUniformMatrix4fv(
        glGetUniformLocation(pimpl_->program, "view"), 1, GL_FALSE,
        glm::value_ptr(camera_info->GetViewMatrix(*camera_transform)));
    glUniformMatrix4fv(glGetUniformLocation(pimpl_->program, "projection"), 1,
                       GL_FALSE,
                       glm::value_ptr(camera_info->GetProjectionMatrix()));
    for (entityx::Entity model :
         es.entities_with_components(model_transform, model_info)) {
      for (int i = 0; i < model_info->pimpl_->meshes_.size(); ++i) {
        auto model = model_transform->GetWorldMatrix();
        glUniformMatrix4fv(glGetUniformLocation(pimpl_->program, "model"), 1,
                           GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(pimpl_->program, "texture_sampler"),
                    model_info->pimpl_->meshes_[i].texture->texture_unit_id_ -
                        GL_TEXTURE0);
        glBindVertexArray(model_info->pimpl_->meshes_[i].vao);
        glActiveTexture(
            model_info->pimpl_->meshes_[i].texture->texture_unit_id_);
        glBindTexture(GL_TEXTURE_2D,
                      model_info->pimpl_->meshes_[i].texture->texture_id_);

        glDrawElements(GL_TRIANGLES, model_info->pimpl_->meshes_[i].num_indices,
                       GL_UNSIGNED_INT, 0);
      }
    }
  }
}
