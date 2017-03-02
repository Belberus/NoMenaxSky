#pragma once
#include <vector>
#include <functional>
#include "Actor.h"
class Scene
{
	std::vector<std::reference_wrapper<Actor>> actors;
public:
	Scene();
	void addActor(Actor &actor);
	void draw();
	~Scene();
};

