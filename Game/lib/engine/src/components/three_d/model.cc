#include "engine/components/three_d/model.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "components/three_d/model_impl.h"

engine::components::three_d::Model::Model(const std::string& filename)
    : pimpl_(new engine::components::three_d::impl::ModelImpl()) {
  Assimp::Importer importer;
  auto scene = importer.ReadFile(
      filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
  if (!scene) {
    throw std::runtime_error(
        "Error while processing the file containing the model: " + filename);
  }
  pimpl_->InitModel(*scene, filename);
}

engine::components::three_d::Model::Model(const Model& model)
    : pimpl_(std::make_unique<impl::ModelImpl>(*model.pimpl_)) {}

engine::components::three_d::Model& engine::components::three_d::Model::
operator=(const Model& model) {
  *pimpl_ = *model.pimpl_;
  return *this;
}

engine::components::three_d::Model::Model(Model&& model) = default;

engine::components::three_d::Model& engine::components::three_d::Model::
operator=(Model&& model) = default;

engine::components::three_d::Model::~Model() = default;