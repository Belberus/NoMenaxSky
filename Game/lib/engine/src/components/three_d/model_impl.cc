#include "components/three_d/model_impl.h"

#include <iostream>

engine::components::three_d::impl::ModelImpl::Mesh::Mesh(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices, int material_index)
    : material_index(material_index), num_indices(indices.size()) {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vertex_buf);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT),
                        (void*)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(GL_FLOAT),
                        (void*)(6 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(2);
  glGenBuffers(1, &indices_buf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);
  glBindVertexArray(0);
}

engine::components::three_d::impl::ModelImpl::Mesh::~Mesh() {}

void engine::components::three_d::impl::ModelImpl::InitModel(
    const aiScene& scene, const std::string& path) {
  meshes_.reserve(scene.mNumMeshes);
  for (int i = 0; i < scene.mNumMeshes; ++i) {
    meshes_.push_back(InitMesh(scene, *scene.mMeshes[i], i));
  }
  materials_ = InitMaterials(scene, path);
}

engine::components::three_d::impl::ModelImpl::Mesh
engine::components::three_d::impl::ModelImpl::InitMesh(const aiScene& scene,
                                                       const aiMesh& mesh,
                                                       const int mesh_index) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  for (int i = 0; i < mesh.mNumVertices; ++i) {
    auto pos = mesh.mVertices[i];
    auto normal = mesh.mNormals[i];
    auto tex_coord = mesh.HasTextureCoords(0) ? mesh.mTextureCoords[0][i]
                                              : aiVector3D(0.0f, 0.0f, 0.0f);
    Vertex vertex;
    vertex.position = glm::vec3(pos.x, pos.y, pos.z);
    vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
    vertex.tex_coordinates = glm::vec2(tex_coord.x, tex_coord.y);
    vertices.push_back(vertex);
  }

  for (int i = 0; i < mesh.mNumFaces; ++i) {
    auto face = mesh.mFaces[i];
    for (int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }
  return Mesh(vertices, indices, mesh.mMaterialIndex);
}

std::vector<std::shared_ptr<engine::components::two_d::Texture>>
engine::components::three_d::impl::ModelImpl::InitMaterials(
    const aiScene& scene, const std::string& path) {
  auto directory = path.substr(0, path.find_last_of('/'));
  std::vector<std::shared_ptr<engine::components::two_d::Texture>> materials;
  materials.reserve(scene.mNumMaterials);
  for (int i = 0; i < scene.mNumMaterials; ++i) {
    auto material = scene.mMaterials[i];
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      aiString path;
      if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr,
                               nullptr, nullptr, nullptr,
                               nullptr) == AI_SUCCESS) {
        materials.emplace_back(new engine::components::two_d::Texture(
            directory + '/' + std::string(path.C_Str())));
      }
    } else {
      materials.emplace_back(nullptr);
    }
  }
  return materials;
}

glm::mat4 engine::components::three_d::impl::ModelImpl::ConvertToGlmMatrix(
    const aiMatrix4x4& from) {
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
