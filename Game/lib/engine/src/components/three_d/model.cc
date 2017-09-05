#include "engine/components/three_d/model.h"

#include "components/three_d/model_impl.h"

engine::components::three_d::Model::Model(const std::string& filename)
    : pimpl_(new engine::components::three_d::impl::ModelImpl()) {
  pimpl_->LoadModel(filename);
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

void engine::components::three_d::Model::PlayAnimation(
    const std::string& animation_name, bool loop) {
  pimpl_->PlayAnimation(animation_name, loop);
}
