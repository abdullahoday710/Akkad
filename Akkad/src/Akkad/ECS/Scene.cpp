#include "Scene.h"
#include "Entity.h"

#include "Components/TagComponent.h"
#include "Components/TransformComponent.h"

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
		// any entity must have a tag and transform component by default !
		auto& tag_comp = entity.AddComponent<TagComponent>();
		auto& transform_comp = entity.AddComponent<TransformComponent>();
		tag_comp.Tag = tag;

		return entity;
	}

}

