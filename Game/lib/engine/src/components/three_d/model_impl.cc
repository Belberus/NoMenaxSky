#include "components/three_d/model_impl.h"

#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "engine/core/engine.h"
#include "engine/core/resource_manager.h"

engine::components::three_d::impl::ModelImpl::Mesh::Mesh(
    const std::string &path, const aiScene &scene, unsigned int mesh_index,
    const aiMatrix4x4 &transform)
    : id_counter(0), bone_transforms(100, glm::mat4(1.0f)) {
  texture = LoadTexture(path, scene, mesh_index);
  auto vertices = LoadVertexInfo(scene, mesh_index, transform);
  auto indices = LoadIndices(scene, mesh_index);
  num_indices = indices.size();
  auto bones_info = LoadBoneInfo(scene, mesh_index);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vertex_buf);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glGenBuffers(1, &bones_buf);
  glBindBuffer(GL_ARRAY_BUFFER, bones_buf);
  glBufferData(GL_ARRAY_BUFFER, bones_info.size() * sizeof(BoneInfo),
               bones_info.data(), GL_STATIC_DRAW);
  glVertexAttribIPointer(3, 4, GL_INT, sizeof(BoneInfo), (void *)0);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(BoneInfo),
                        (void *)(4 * sizeof(int)));
  glEnableVertexAttribArray(4);
  glGenBuffers(1, &indices_buf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);
  glBindVertexArray(0);
}

std::vector<engine::components::three_d::impl::ModelImpl::Mesh::BoneInfo>
engine::components::three_d::impl::ModelImpl::Mesh::LoadBoneInfo(
    const aiScene &scene, unsigned int mesh_index) {
  auto mesh = scene.mMeshes[mesh_index];
  if (mesh->mNumBones == 0) {
    has_skeleton = false;
    BoneInfo info;
    info.AddBoneInfo(0, 1.0f);
    return std::vector<BoneInfo>(mesh->mNumVertices, info);
  }
  has_skeleton = true;
  std::vector<Mesh::BoneInfo> bone_info(mesh->mNumVertices);
  for (int i = 0; i < mesh->mNumBones; ++i) {
    auto bone = mesh->mBones[i];
    bone_offsets.push_back(ConvertToGlmMatrix(bone->mOffsetMatrix));
    name_id_map.emplace(bone->mName.data, id_counter++);
    for (int j = 0; j < bone->mNumWeights; ++j) {
      auto weight_info = bone->mWeights[j];
      BoneInfo &bone_inf = bone_info[weight_info.mVertexId];
      bone_inf.AddBoneInfo(id_counter - 1, weight_info.mWeight);
    }
  }
  return bone_info;
}

std::vector<engine::components::three_d::impl::ModelImpl::Mesh::Vertex>
engine::components::three_d::impl::ModelImpl::Mesh::LoadVertexInfo(
    const aiScene &scene, unsigned int mesh_index,
    const aiMatrix4x4 &transform) {
  std::vector<Mesh::Vertex> vertices;
  auto mesh = scene.mMeshes[mesh_index];
  for (int i = 0; i < mesh->mNumVertices; ++i) {
    auto pos = mesh->mVertices[i];
    auto normal = mesh->mNormals[i];
    auto tex_coord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i]
                                               : aiVector3D(0.0f, 0.0f, 0.0f);
    auto transform_mat = ConvertToGlmMatrix(transform);
    auto final_pos = transform_mat * glm::vec4(pos.x, pos.y, pos.z, 1.0f);
    Mesh::Vertex vertex;
    vertex.position = final_pos;
    vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
    vertex.tex_coordinates = glm::vec2(tex_coord.x, tex_coord.y);
    vertices.push_back(vertex);
  }
  return vertices;
}

std::vector<unsigned int>
engine::components::three_d::impl::ModelImpl::Mesh::LoadIndices(
    const aiScene &scene, unsigned int mesh_index) {
  std::vector<unsigned int> indices;
  auto mesh = scene.mMeshes[mesh_index];
  for (int i = 0; i < mesh->mNumFaces; ++i) {
    auto face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }
  return indices;
}

std::shared_ptr<engine::components::two_d::Texture>
engine::components::three_d::impl::ModelImpl::Mesh::LoadTexture(
    const std::string &path, const aiScene &scene, unsigned int mesh_index) {
  std::shared_ptr<engine::components::two_d::Texture> texture;
  auto directory = path.substr(0, path.find_last_of('/'));
  auto material_index = scene.mMeshes[mesh_index]->mMaterialIndex;
  auto material = scene.mMaterials[material_index];
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

void engine::components::three_d::impl::ModelImpl::Mesh::ReplaceBoneTransform(
    const std::string &bone_name, const glm::mat4 &new_transform) {
  if (name_id_map.count(bone_name) > 0) {
    auto bone_id = name_id_map[bone_name];
    bone_transforms[bone_id] = new_transform * bone_offsets[bone_id];
  }
}

engine::components::three_d::impl::ModelImpl::ModelImpl()
    : time_passed_(0.0f), loop_(false), playing_animation_(nullptr) {}

void engine::components::three_d::impl::ModelImpl::LoadModel(
    const std::string &path) {
  path_ = path;
  scene_ = importer_.ReadFile(path.c_str(), aiProcess_Triangulate |
                                                aiProcess_GenSmoothNormals |
                                                aiProcess_LimitBoneWeights);
  if (!scene_) {
    throw std::runtime_error(
        "Error while processing the file containing the model: " + path + ". " +
        importer_.GetErrorString());
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
    meshes_.emplace_back(path_, *scene_, mesh_index, transform);
  }
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

void engine::components::three_d::impl::ModelImpl::PlayAnimation(
    const std::string &animation_name, bool loop) {
  auto animation = FindAnimation(animation_name);
  // ensure there are animations with that name
  assert(animation != nullptr);
  playing_animation_ = animation;
  loop_ = loop;
  time_passed_ = 0.0f;
}

void engine::components::three_d::impl::ModelImpl::Animate(float dt) {
  if (playing_animation_) {
    float ticks_per_second = 42.0f;
    if (playing_animation_->mTicksPerSecond != 0) {
      ticks_per_second = playing_animation_->mTicksPerSecond;
    }
    auto duration_in_sec =
        playing_animation_->mDuration * (1.0f / ticks_per_second);
    time_passed_ += dt;
    if (time_passed_ >= duration_in_sec) {
      if (loop_) {
        time_passed_ = time_passed_ - duration_in_sec;
      } else {
        time_passed_ = 0.0f;
        playing_animation_ = nullptr;
        return;
      }
    }
    auto time_passsed_ticks = time_passed_ * ticks_per_second;
    TraverseBoneTree(time_passsed_ticks, *playing_animation_, glm::mat4(),
                     scene_->mRootNode);
  }
}

glm::vec3 CalcInterpolatedScale(float time_passed_ticks,
                                const aiNodeAnim &channel) {
  if (channel.mNumScalingKeys == 1) {
    glm::vec3 scale;
    scale.x = channel.mScalingKeys[0].mValue.x;
    scale.y = channel.mScalingKeys[0].mValue.y;
    scale.z = channel.mScalingKeys[0].mValue.z;
    return scale;
  }
  int index = 0;
  for (int i = 0; i < channel.mNumScalingKeys - 1; ++i) {
    if (time_passed_ticks < channel.mScalingKeys[i + 1].mTime) {
      index = i;
      break;
    }
  }
  float delta_time =
      channel.mScalingKeys[index + 1].mTime - channel.mScalingKeys[index].mTime;
  float factor =
      (time_passed_ticks - channel.mScalingKeys[index].mTime) / delta_time;
  glm::vec3 starting_scale;
  starting_scale.x = channel.mScalingKeys[index].mValue.x;
  starting_scale.y = channel.mScalingKeys[index].mValue.y;
  starting_scale.z = channel.mScalingKeys[index].mValue.z;
  glm::vec3 ending_scale;
  ending_scale.x = channel.mScalingKeys[index + 1].mValue.x;
  ending_scale.y = channel.mScalingKeys[index + 1].mValue.y;
  ending_scale.z = channel.mScalingKeys[index + 1].mValue.z;
  return glm::mix(starting_scale, ending_scale, factor);
}

glm::vec3 CalcInterpolatedPosition(float time_passed_ticks,
                                   const aiNodeAnim &channel) {
  if (channel.mNumPositionKeys == 1) {
    glm::vec3 pos;
    pos.x = channel.mPositionKeys[0].mValue.x;
    pos.y = channel.mPositionKeys[0].mValue.y;
    pos.z = channel.mPositionKeys[0].mValue.z;
    return pos;
  }
  int index = 0;
  for (int i = 0; i < channel.mNumPositionKeys - 1; ++i) {
    if (time_passed_ticks < channel.mPositionKeys[i + 1].mTime) {
      index = i;
      break;
    }
  }
  float delta_time = channel.mPositionKeys[index + 1].mTime -
                     channel.mPositionKeys[index].mTime;
  float factor =
      (time_passed_ticks - channel.mPositionKeys[index].mTime) / delta_time;
  glm::vec3 starting_pos;
  starting_pos.x = channel.mPositionKeys[index].mValue.x;
  starting_pos.y = channel.mPositionKeys[index].mValue.y;
  starting_pos.z = channel.mPositionKeys[index].mValue.z;
  glm::vec3 ending_pos;
  ending_pos.x = channel.mPositionKeys[index + 1].mValue.x;
  ending_pos.y = channel.mPositionKeys[index + 1].mValue.y;
  ending_pos.z = channel.mPositionKeys[index + 1].mValue.z;
  return glm::mix(starting_pos, ending_pos, factor);
}

glm::quat CalcInterpolatedRotation(float time_passed_ticks,
                                   const aiNodeAnim &channel) {
  if (channel.mNumRotationKeys == 1) {
    glm::quat rotation;
    rotation.w = channel.mRotationKeys[0].mValue.w;
    rotation.z = channel.mRotationKeys[0].mValue.z;
    rotation.y = channel.mRotationKeys[0].mValue.y;
    rotation.x = channel.mRotationKeys[0].mValue.x;
    return rotation;
  }
  int index = 0;
  for (int i = 0; i < channel.mNumRotationKeys - 1; ++i) {
    if (time_passed_ticks < channel.mRotationKeys[i + 1].mTime) {
      index = i;
      break;
    }
  }
  float delta_time = channel.mRotationKeys[index + 1].mTime -
                     channel.mRotationKeys[index].mTime;
  float factor =
      (time_passed_ticks - channel.mRotationKeys[index].mTime) / delta_time;
  aiQuaternion interpolated_rotation;
  aiQuaternion::Interpolate(interpolated_rotation,
                            channel.mRotationKeys[index].mValue,
                            channel.mRotationKeys[index + 1].mValue, factor);
  glm::quat glm_quat;
  glm_quat.w = interpolated_rotation.w;
  glm_quat.z = interpolated_rotation.z;
  glm_quat.y = interpolated_rotation.y;
  glm_quat.x = interpolated_rotation.x;
  return glm_quat;
}

void engine::components::three_d::impl::ModelImpl::TraverseBoneTree(
    float time_passed_ticks, const aiAnimation &animation,
    glm::mat4 acc_transform, const aiNode *node) {
  std::string node_name(node->mName.data);
  auto node_transform = ConvertToGlmMatrix(node->mTransformation);
  auto channel = FindChannel(animation, node_name);
  if (channel) {
    auto interpolated_scale =
        CalcInterpolatedScale(time_passed_ticks, *channel);
    auto interpolated_position =
        CalcInterpolatedPosition(time_passed_ticks, *channel);
    auto interpolated_rotation =
        CalcInterpolatedRotation(time_passed_ticks, *channel);
    auto scale_mat = glm::scale(glm::mat4(), interpolated_scale);
    auto position_mat = glm::translate(glm::mat4(), interpolated_position);
    auto rotation_mat = glm::mat4_cast(interpolated_rotation);
    node_transform = position_mat * rotation_mat * scale_mat;
  }
  acc_transform = acc_transform * node_transform;
  for (auto &mesh : meshes_) {
    mesh.ReplaceBoneTransform(node_name, acc_transform);
  }
  for (int i = 0; i < node->mNumChildren; ++i) {
    TraverseBoneTree(time_passed_ticks, animation, acc_transform,
                     node->mChildren[i]);
  }
}

aiAnimation *engine::components::three_d::impl::ModelImpl::FindAnimation(
    const std::string &name) {
  for (int i = 0; i < scene_->mNumAnimations; ++i) {
    auto animation = scene_->mAnimations[i];
    if (std::string(animation->mName.data) == name) {
      return animation;
    }
  }
  return nullptr;
}

aiNodeAnim *engine::components::three_d::impl::ModelImpl::FindChannel(
    const aiAnimation &animation, const std::string &name) {
  for (int i = 0; i < animation.mNumChannels; ++i) {
    auto channel = animation.mChannels[i];
    if (std::string(channel->mNodeName.data) == name) {
      return channel;
    }
  }
  return nullptr;
}