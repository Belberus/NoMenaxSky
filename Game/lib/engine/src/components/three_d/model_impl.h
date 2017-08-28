#ifndef ENGINE_COMPONENTS_THREE_D_MODEL_IMPL_H_
#define ENGINE_COMPONENTS_THREE_D_MODEL_IMPL_H_

#include <GL/gl3w.h>

#include <memory>
#include <vector>

#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "engine/components/two_d/texture.h"

namespace engine {
namespace components {
namespace three_d {
namespace impl {
/// This class is used as the implementaion of the Model class. It used mostly
/// to hide OpenGL types from the client using the Model class.
class ModelImpl {
 public:
  /// Vertex info passed to the vertex shaders
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coordinates;
  };
  /// Class that represents a Mesh
  struct Mesh {
    Mesh();
    Mesh(const std::vector<Vertex> &vertices,
         const std::vector<unsigned int> &indices, int material_index);
    ~Mesh();
    GLuint vao;
    GLuint indices_buf;
    GLuint vertex_buf;
    GLsizei num_indices;
    int material_index;
  };

  /// Initializes all our 3D model data structure with all the meshes.
  /// @param scene data structure containing the info about the imported model.
  void InitModel(const aiScene &scene, const std::string &path);

  /// Initializes our mesh data structure.
  /// @param mesh data structure containing the info about the imported mesh.
  /// @return the imported mesh but in our own data structure.
  Mesh InitMesh(const aiScene &scene, const aiMesh &mesh, const int mesh_index);

  /// Initializes our texture data structure.
  /// @param scene data structure containing the info about the 3D model and all
  /// of the possible materials.
  /// @return the model materials in our own data structure.
  std::vector<std::shared_ptr<engine::components::two_d::Texture>>
  InitMaterials(const aiScene &scene, const std::string &path);

  /// Converts from an aiMatrix4x4 to a glm::mat4.
  /// @param from matrix to be converted.
  /// @return the input matrix in glm::mat4 format.
  glm::mat4 ConvertToGlmMatrix(const aiMatrix4x4 &from);

  std::vector<Mesh> meshes_;
  std::vector<std::shared_ptr<engine::components::two_d::Texture>> materials_;
};
}  // namespace impl
}  // namespace three_d
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_THREE_D_MODEL_IMPL_H_