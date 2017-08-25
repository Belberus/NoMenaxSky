#include "engine/components/three_d/model.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

struct engine::components::three_d::Model::Impl {};

engine::components::three_d::Model::Model(const std::string& filename) {
  Assimp::Importer importer;
  auto scene = importer.ReadFile(
      filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
  if (!scene) {
    throw std::runtime_error(
        "Error while opening the file containing the model: " + filename);
  }
}
