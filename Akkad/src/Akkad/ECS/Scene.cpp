#include "Scene.h"
#include "Entity.h"

#include "Components/TagComponent.h"

namespace Akkad {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Update()
	{
	}

	Entity Scene::AddEntity(std::string tag)
	{
		Entity entity = { m_Registry.create(), this };
		auto& tag_comp = entity.AddComponent<TagComponent>();
		tag_comp.Tag = tag;

		return entity;
	}

}

