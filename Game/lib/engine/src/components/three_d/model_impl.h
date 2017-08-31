#ifndef ENGINE_COMPONENTS_THREE_D_MODEL_IMPL_H_
#define ENGINE_COMPONENTS_THREE_D_MODEL_IMPL_H_

#include <GL/gl3w.h>

#include <memory>
#include <vector>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
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
  /// Class that represents a Mesh
  struct Mesh {
    /// Vertex info passed to the vertex shaders
    struct Vertex {
      glm::vec3 position;
      glm::vec3 normal;
      glm::vec2 tex_coordinates;
    };
    Mesh(const std::vector<Vertex> &vertices,
         const std::vector<unsigned int> &indices,
         const std::shared_ptr<engine::components::two_d::Texture> &texture);
    GLuint vao;
    GLuint indices_buf;
    GLuint vertex_buf;
    GLsizei num_indices;
    std::shared_ptr<engine::components::two_d::Texture> texture;
  };

  /// Loads the 3D model from the specified file. A model is a set of meshes.
  /// @param path path where the model is located.
  void LoadModel(const std::string &path);
  /// Traverses the tree in search of meshes to load. This mantains the
  /// relationships between meshes that exsit.
  /// @param transforms the transforms of the nodes that have been visited.
  /// @param node tree node to traverse.
  void LoadModelAux(std::vector<aiMatrix4x4> transforms, aiNode *node);
  /// Copies the meshes that there can be in a node inside our data structure
  /// meshes_.
  /// @param transform the final transform of that node; final because its the
  /// multiplication of this node transform with the transforms his ancestors.
  void LoadMeshes(const aiNode &node, const aiMatrix4x4 &transform);
  /// Loads a mesh.
  /// @param mesh_index the location of a mesh in the meshes array inside an
  /// aiScene object.
  /// @param transform the final transform of this mesh.
  /// @return the new mesh in our own format, ready to be rendered.
  Mesh LoadMesh(unsigned int mesh_index, const aiMatrix4x4 &transform);
  /// Loads a texture.
  /// @param material_index the location of a material in the materials array
  /// inside an aiScene object.
  /// @return new texture.
  std::shared_ptr<engine::components::two_d::Texture> LoadTexture(
      unsigned int material_index);
  /// Converts from a aiMatrix4x4 to a glm::mat4.
  /// @param from matrix to be converted.
  /// @return the specified matrix converted to glm::mat4.
  glm::mat4 ConvertToGlmMatrix(const aiMatrix4x4 &from);

  Assimp::Importer importer_;
  const aiScene *scene_;
  std::vector<Mesh> meshes_;
  std::string path_;
};
}  // namespace impl
}  // namespace three_d
}  // namespace components
}  // namespace engine
#endif  // ENGINE_COMPONENTS_THREE_D_MODEL_IMPL_H_