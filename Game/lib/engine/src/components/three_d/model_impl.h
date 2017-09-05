#ifndef ENGINE_COMPONENTS_THREE_D_MODEL_IMPL_H_
#define ENGINE_COMPONENTS_THREE_D_MODEL_IMPL_H_

#include <GL/gl3w.h>

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
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
/// This class is used as the implementation of the Model class. It is used
/// mostly to hide OpenGL types from the client using the Model class.
class ModelImpl {
 public:
  /// Class that represents a Mesh
  struct Mesh {
    /// Vertex info passed to the vertex shaders per vertex
    struct Vertex {
      glm::vec3 position;
      glm::vec3 normal;
      glm::vec2 tex_coordinates;
    };
    /// Bone info passed to the vertex shader per vertex. It consists of the
    /// bones affecting the vertex and the amout of influence each bone has on
    /// the vertex.
    struct BoneInfo {
      BoneInfo() {
        std::fill_n(bone_id, 4, 0);
        std::fill_n(bone_weight, 4, 0.0f);
      }
      /// Adds bone information.
      /// @param id id of the bone.
      /// @param weight influce of the bone on the vertex.
      void AddBoneInfo(int id, float weight) {
        for (int i = 0; i < 4; ++i) {
          if (bone_weight[i] == 0.0f) {
            bone_id[i] = id;
            bone_weight[i] = weight;
          }
        }
      }
      int bone_id[4];
      float bone_weight[4];
    };
    /// Mesh constructor.
    /// @param path path of the file containing the model mesh.
    /// @param scene scene of the imported model.
    /// @param mesh_index identifier of the mesh inside the aiScene specified.
    /// @param transform the transform of this mesh.
    Mesh(const std::string &path, const aiScene &scene, unsigned int mesh_index,
         const aiMatrix4x4 &transform);
    /// Loads the information about the bones of the mesh.
    /// @param scene scene of the imported model.
    /// @param mesh_index identifier of the mesh inside the aiScene spcified.
    /// @return a vector with the same size as the number of vertices of the
    /// mesh that contains information about the bones and how it affects the
    /// vertex. It is indexed by the vertex id. If the mesh doesnt have bones
    /// the same array if returned buy with default values for BoneInfo.
    std::vector<BoneInfo> LoadBoneInfo(const aiScene &scene,
                                       unsigned int mesh_index);
    /// Loads the information about the vertices of the mesh.
    /// @param scene scene of the imported model.
    /// @param mesh_index identifier of the mesh inside the aiScene spcified.
    /// @param transform the transform of this mesh.
    /// @return vector with all the information about all the vertices.
    std::vector<Vertex> LoadVertexInfo(const aiScene &scene,
                                       unsigned int mesh_index,
                                       const aiMatrix4x4 &transform);
    /// Loads the indices of the mesh.
    /// @param scene scene of the imported model.
    /// @param mesh_index identifier of the mesh inside the aiScene spcified.
    /// @return vector with the indices of the mesh.
    std::vector<unsigned int> LoadIndices(const aiScene &scene,
                                          unsigned int mesh_index);
    /// Loads a texture of the mesh.
    /// @param path path of the file containing the model mesh.
    /// @param scene scene of the imported model.
    /// @param mesh_index identifier of the mesh inside the aiScene spcified.
    /// @return the diffuse texture of this mesh.
    std::shared_ptr<engine::components::two_d::Texture> LoadTexture(
        const std::string &path, const aiScene &scene, unsigned int mesh_index);
    /// Replaces a bone transform with a new one if the mesh has that bone.
    /// @param bone_name name of the bone whose ransform will be replaced.
    /// @param new_transform the new transform of the bone specified.
    void ReplaceBoneTransform(const std::string &bone_name,
                              const glm::mat4 &new_transform);
    const static int kNumBonesPerVertex = 4;
    GLuint vao;
    GLuint indices_buf;
    GLuint vertex_buf;
    GLuint bones_buf;
    GLsizei num_indices;
    int id_counter;
    bool has_skeleton;
    std::vector<glm::mat4> bone_transforms;
    std::vector<glm::mat4> bone_offsets;
    std::unordered_map<std::string, int> name_id_map;
    std::shared_ptr<engine::components::two_d::Texture> texture;
  };

  ModelImpl();

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
  /// Converts from a aiMatrix4x4 to a glm::mat4.
  /// @param from matrix to be converted.
  /// @return the specified matrix converted to glm::mat4.
  static glm::mat4 ConvertToGlmMatrix(const aiMatrix4x4 &from);

  // TODO: put the animation related methods and variables inside their own
  // class, like a SekeltonAnimator
  /// Sets the needed information so the specifed animation can be played.
  /// @param animation_name name of the animation.
  /// @param loop if true the animation will be played over and over again, if
  /// false it will be played once.
  void PlayAnimation(const std::string &animation_name, bool loop);
  /// Calculates all of the bones transforms based on the animation that should
  /// be played.
  /// @param dt time in seconds represeting how much time the last frame took to
  /// render.
  void Animate(float dt);
  /// Traverses the tree of the scene calculating the bones transforms and
  /// updating them  in each mesh based on the animation that should be played
  /// and the time that has passed.
  /// @param time_passsed_tics the time that has passed (in ticks) since the
  /// animation started
  /// @param animation the animation that should be played.
  /// @param acc_transform the accumulated transform formed by traversing the
  /// tree.
  /// @param node pointer to the root node.
  void TraverseBoneTree(float time_passed_ticks, const aiAnimation &animation,
                        glm::mat4 acc_transform, const aiNode *node);
  aiAnimation *FindAnimation(const std::string &name);
  aiNodeAnim *FindChannel(const aiAnimation &animation,
                          const std::string &name);
  float time_passed_;
  const aiAnimation *playing_animation_;
  bool loop_;

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