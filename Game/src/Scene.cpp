#include "Scene.h"

Scene::Scene()
{
}

void Scene::addActor(Actor &actor) {
	actors.push_back(actor);
}

void Scene::draw() {
	for (auto a : actors) {
		a.get().draw();
	}
}


Scene::~Scene()
{
}
