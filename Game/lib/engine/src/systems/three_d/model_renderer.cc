#include <GL/gl3w.h>

#include "engine/systems/three_d/model_renderer.h"

#include <glm/gtc/type_ptr.hpp>

#include "components/three_d/model_impl.h"
#include "engine/components/common/camera.h"
#include "engine/components/common/transform.h"
#include "engine/components/three_d/model.h"
#include "utils/shaders.h"

struct engine::systems::three_d::ModelRenderer::Impl {
  Impl();
  // TODO: consider creating two separated systems one for animated meshes and
  // another for non animated meshes in order to be a bit more efficient.
  // Right now, if a mesh doesnt have skeleton we keep allocating 100 identiy
  // matrices and sending them to the vertex shader.
  void RenderSkeletalAnimatedMeshes(entityx::EntityManager &entities,
                                    entityx::EventManager &events,
                                    entityx::TimeDelta dt);
  static const GLchar *vert_shader_skeleton_src;
  static const GLchar *frag_shader_src;
  GLuint skeleton_program;
};

const GLchar
    *engine::systems::three_d::ModelRenderer::Impl::vert_shader_skeleton_src =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 normal;"
        "layout (location = 2) in vec2 tex_coord;\n"
        "layout (location = 3) in ivec4 bones_id;\n"
        "layout (location = 4) in vec4 bones_weight;\n"
        "out vec2 TexCoord;\n"
        "const int MAX_BONES = 100;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform vec4 in_color;\n"
        "uniform mat4 bones_transforms[MAX_BONES];\n"
        "void main(void)\n"
        "{\n"
        "mat4 bone = bones_transforms[bones_id[0]] * bones_weight[0];\n"
        "bone += bones_transforms[bones_id[1]] * bones_weight[1];\n"
        "bone += bones_transforms[bones_id[2]] * bones_weight[2];\n"
        "bone += bones_transforms[bones_id[3]] * bones_weight[3];\n"
        "gl_Position = projection * view * model * bone * vec4(position, "
        "1.0);\n"
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

engine::systems::three_d::ModelRenderer::Impl::Impl() {
  GLuint vert_skeleton =
      engine::utils::CompileShader(vert_shader_skeleton_src, GL_VERTEX_SHADER);
  GLuint frag =
      engine::utils::CompileShader(frag_shader_src, GL_FRAGMENT_SHADER);
  skeleton_program = engine::utils::LinkProgram(vert_skeleton, frag);
  glDeleteShader(vert_skeleton);
  glDeleteShader(frag);
}

void engine::systems::three_d::ModelRenderer::Impl::
    RenderSkeletalAnimatedMeshes(entityx::EntityManager &entities,
                                 entityx::EventManager &events,
                                 entityx::TimeDelta dt) {
  entityx::ComponentHandle<engine::components::common::Transform>
      camera_transform;
  entityx::ComponentHandle<engine::components::common::Camera> camera_info;
  glUseProgram(skeleton_program);
  glUniform4fv(glGetUniformLocation(skeleton_program, "in_color"), 1,
               glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));
  for (entityx::Entity camera :
       entities.entities_with_components(camera_transform, camera_info)) {
    entityx::ComponentHandle<engine::components::common::Transform>
        model_transform;
    entityx::ComponentHandle<engine::components::three_d::Model> model_info;
    glUniformMatrix4fv(
        glGetUniformLocation(skeleton_program, "view"), 1, GL_FALSE,
        glm::value_ptr(camera_info->GetViewMatrix(*camera_transform)));
    glUniformMatrix4fv(glGetUniformLocation(skeleton_program, "projection"), 1,
                       GL_FALSE,
                       glm::value_ptr(camera_info->GetProjectionMatrix()));
    for (entityx::Entity model :
         entities.entities_with_components(model_transform, model_info)) {
      model_info->pimpl_->Animate(dt);
      for (int i = 0; i < model_info->pimpl_->meshes_.size(); ++i) {
        auto model = model_transform->GetWorldMatrix();
        glUniformMatrix4fv(glGetUniformLocation(skeleton_program, "model"), 1,
                           GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(
            glGetUniformLocation(skeleton_program, "bones_transforms[0]"), 100,
            GL_FALSE,
            glm::value_ptr(model_info->pimpl_->meshes_[i].bone_transforms[0]));
        glUniform1i(glGetUniformLocation(skeleton_program, "texture_sampler"),
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

engine::systems::three_d::ModelRenderer::ModelRenderer() : pimpl_(new Impl()) {}

engine::systems::three_d::ModelRenderer::ModelRenderer(
    const ModelRenderer &renderer)
    : pimpl_(std::make_unique<Impl>(*renderer.pimpl_)) {}

engine::systems::three_d::ModelRenderer &
engine::systems::three_d::ModelRenderer::operator=(
    const ModelRenderer &renderer) {
  *pimpl_ = *renderer.pimpl_;
  return *this;
}

engine::systems::three_d::ModelRenderer::~ModelRenderer() {}

void engine::systems::three_d::ModelRenderer::update(
    entityx::EntityManager &es, entityx::EventManager &events,
    entityx::TimeDelta dt) {
  pimpl_->RenderSkeletalAnimatedMeshes(es, events, dt);
}
