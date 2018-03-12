#include "pch.h"
#include "Scene.h"

GameObject2D & Scene::getObjectByIndex(int index) const
{
	return *game_objects.at(index);
}

GameObject2D & Scene::getObjectByTag(std::string tag) const
{
	for (auto& obj : game_objects)
	{
		if (obj->GetTag() == tag)
		{
			return *obj;
		}
	}
}
