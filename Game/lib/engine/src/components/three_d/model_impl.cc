#include "components/three_d/model_impl.h"

#include "engine/core/engine.h"
#include "engine/core/resource_manager.h"

engine::components::three_d::impl::ModelImpl::Mesh::Mesh(
    const std::vector<Vertex> &vertices,
    const std::vector<unsigned int> &indices,
    const std::shared_ptr<engine::components::two_d::Texture> &texture)
    : num_indices(indices.size()), texture(texture) {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vertex_buf);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT),
                        (void *)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GL_FLOAT),
                        (void *)(6 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(2);
  glGenBuffers(1, &indices_buf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);
  glBindVertexArray(0);
}

void engine::components::three_d::impl::ModelImpl::LoadModel(
    const std::string &path) {
  path_ = path;
  scene_ = importer_.ReadFile(path.c_str(), aiProcess_Triangulate |
                                                aiProcess_GenSmoothNormals |
                                                aiProcess_LimitBoneWeights);
  if (!scene_) {
    throw std::runtime_error(
        "Error while processing the file containing the model: " + path);
  }
  // for some reason the root node transformation is not an identity matrix
  scene_->mRootNode->mTransformation = aiMatrix4x4();
  LoadModelAux({aiMatrix4x4()}, scene_->mRootNode);
}

void engine::components::three_d::impl::ModelImpl::LoadModelAux(
    std::vector<aiMatrix4x4> transforms, aiNode *node) {
  transforms.push_back(node->mTransformation);
  if (node->mNumMeshes > 0) {
    aiMatrix4x4 acc_transform;
    for (int i = transforms.size() - 1; i >= 0; --i) {
      acc_transform = transforms[i] * acc_transform;
    }
    LoadMeshes(*node, acc_transform);
  }
  for (int i = 0; i < node->mNumChildren; ++i) {
    LoadModelAux(transforms, node->mChildren[i]);
  }
}

void engine::components::three_d::impl::ModelImpl::LoadMeshes(
    const aiNode &node, const aiMatrix4x4 &transform) {
  for (int i = 0; i < node.mNumMeshes; ++i) {
    auto mesh_index = node.mMeshes[i];
    meshes_.push_back(LoadMesh(mesh_index, transform));
  }
}

engine::components::three_d::impl::ModelImpl::Mesh
engine::components::three_d::impl::ModelImpl::LoadMesh(
    unsigned int mesh_index, const aiMatrix4x4 &matrix) {
  std::vector<Mesh::Vertex> vertices;
  std::vector<unsigned int> indices;
  auto mesh = scene_->mMeshes[mesh_index];
  for (int i = 0; i < mesh->mNumVertices; ++i) {
    auto pos = mesh->mVertices[i];
    auto normal = mesh->mNormals[i];
    auto tex_coord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i]
                                               : aiVector3D(0.0f, 0.0f, 0.0f);
    aiVector3D scaling;
    aiQuaternion rotation;
    aiVector3D position;
    matrix.Decompose(scaling, rotation, position);
    auto transform = ConvertToGlmMatrix(matrix);
    auto final_pos = transform * glm::vec4(pos.x, pos.y, pos.z, 1.0f);
    Mesh::Vertex vertex;
    vertex.position = final_pos;
    vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
    vertex.tex_coordinates = glm::vec2(tex_coord.x, tex_coord.y);
    vertices.push_back(vertex);
  };

  for (int i = 0; i < mesh->mNumFaces; ++i) {
    auto face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }
  return Mesh(vertices, indices,
              LoadTexture(scene_->mMeshes[mesh_index]->mMaterialIndex));
}

std::shared_ptr<engine::components::two_d::Texture>
engine::components::three_d::impl::ModelImpl::LoadTexture(
    unsigned int material_index) {
  std::shared_ptr<engine::components::two_d::Texture> texture;
  auto directory = path_.substr(0, path_.find_last_of('/'));
  auto material = scene_->mMaterials[material_index];
  if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
    aiString path;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr,
                             nullptr, nullptr, nullptr) == AI_SUCCESS) {
      texture = engine::core::Engine::GetInstance()
                    .Get<engine::core::ResourceManager>()
                    .Load<engine::components::two_d::Texture>(
                        directory + '/' + std::string(path.C_Str()));
    }
  }
  return texture;
}

glm::mat4 engine::components::three_d::impl::ModelImpl::ConvertToGlmMatrix(
    const aiMatrix4x4 &from) {
  glm::mat4 to;
  to[0][0] = (float)from.a1;
  to[0][1] = (float)from.b1;
  to[0][2] = (float)from.c1;
  to[0][3] = (float)from.d1;
  to[1][0] = (float)from.a2;
  to[1][1] = (float)from.b2;
  to[1][2] = (float)from.c2;
  to[1][3] = (float)from.d2;
  to[2][0] = (float)from.a3;
  to[2][1] = (float)from.b3;
  to[2][2] = (float)from.c3;
  to[2][3] = (float)from.d3;
  to[3][0] = (float)from.a4;
  to[3][1] = (float)from.b4;
  to[3][2] = (float)from.c4;
  to[3][3] = (float)from.d4;
  return to;
}
